#version 460

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;


layout (binding = 0) uniform samplerCube Skybox;

layout (binding = 1) uniform sampler2D AlbedoTex;
layout (binding = 2) uniform sampler2D DetailTex;
layout (binding = 3) uniform sampler2D AlphaTex;

uniform float AlphaDiscard;
uniform mat4 ViewMatrix;
uniform mat4 ModelViewMatrix;

uniform struct LightInfo {

    vec4 position;
	vec3 colour;
	float ambient;
	float diffuse;
	float specular;

    // Spot/Point light only
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;

    // Spotlight only
	vec3 direction;
	float cutoffInner; // Phi
	float cutoffOuter; // Gamma
} lights[4];

uniform struct MaterialInfo {

	vec4 colour;

	float ambient;
	float diffuse;
	float specular;
	float shininess;

	float reflectivity;

} material;


vec3 computeLighting(in int lightNo, in vec3 tColour, in vec3 n)
{
    vec3 colourOut = vec3(0.0);

    // --- Prep calculations --- //
    vec4 lPosition = ModelViewMatrix * lights[lightNo].position; // If light position.w == 0, light is a directional light

    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz; // Direction of light ray

    float sDotN = max(dot(-s, Normal), 0.0); // Angle between normal and light ray


    // --- Attenuation calculation --- //
    float lDistance = length(lPosition - Position) * lPosition.w; // Distance from light

    float attenuation = 1.0 / (1.0 + lights[lightNo].attenuationLinear * lDistance + lights[lightNo].attenuationQuadratic * (lDistance * lDistance));


    // --- Spotlight calculation --- //
    float intensity = 1.0f;
    
    vec3 l = lights[lightNo].direction * lights[lightNo].position.w; // direction / point light filter

    if(l != vec3(0.0)) // if light direcction or position.w == 0, light is not a spotlight
    {
        vec3 lDirection = (ModelViewMatrix * vec4(lights[lightNo].direction, 0.0)).xyz; // Light direction in world space

        float theta = dot(s, lDirection); // angle between light ray (from light to fragment)
        float angle = acos(theta); // real angle

        float epsilon = lights[lightNo].cutoffInner - lights[lightNo].cutoffOuter; // fade out angle

        intensity = clamp((angle - lights[lightNo].cutoffOuter) / epsilon, 0.0, 1.0); // intensity = 1.0 inside the inner cone
    }

    // Ambient calculation
    colourOut += ((tColour * material.ambient) * (lights[lightNo].colour * lights[lightNo].ambient)) * attenuation;

    // Diffuse calculation
    colourOut += ((tColour * material.diffuse) * (lights[lightNo].colour * lights[lightNo].diffuse) * sDotN) * intensity * attenuation;

    // Specular calculation
    if(sDotN > 0.0)
    {
        // view vector
        vec3 v = normalize(-Position).xyz; 

        // skybox reflection vector
        vec3 r = normalize(mat3(transpose(ViewMatrix)) * reflect(normalize(Position).xyz, Normal)).xyz; // transpose to get the inverse of ViewMatrix

        // half vector
        vec3 h = normalize(v + -s);

        // Specular highlight calculation
        vec3 Ks = tColour * material.specular;
        vec3 Ls = lights[lightNo].colour * lights[lightNo].specular;
        vec3 reflectionColour = texture(Skybox, r).rgb; // skybox reflection colour

        colourOut += (Ks * (Ls * reflectionColour)) * pow( max( dot(h, n), 0.0), material.shininess) * intensity * attenuation;
    }

    return colourOut;
}


void main() {

    vec3 Colour = vec3(0.0);

    // Get texture pixel
    vec4 albedoTexColour = texture(AlbedoTex, TexCoord);
//    vec4 detailTexColour = texture(DetailTex, TexCoord); // Not proper detail, just secondary albedo
    vec4 alphaMap = texture(AlphaTex, TexCoord);
    
    // Discard fragment based on alpah map
    if(alphaMap.a < AlphaDiscard)
    {
        discard;
    }
    vec3 n = Normal;
//    // Invert face normals if pointing away from camera
//    if(!gl_FrontFacing)
//    {
//        n = -Normal;
//    }

//    // Mix albedo and detail textures
//    vec3 albedoDetail = normalize(mix(albedoTexColour.rgb, detailTexColour.rgb, detailTexColour.a));

    // Mix texture with base model colour (set colour alpha to 0 to discard it)
    vec3 texColour = mix(albedoTexColour.rgb, material.colour.rgb,material.colour.a);

    // Compute lights
    for(int i = 0; i < 4; i++)
    {
        Colour += computeLighting(i, texColour, n);
    }

    FragColor = vec4(Colour, 1.0);
    
}