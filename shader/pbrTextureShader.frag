#version 460

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;


layout (binding = 0) uniform samplerCube Skybox;
layout (binding = 7) uniform samplerCube EnvironmentMap;

layout (binding = 1) uniform sampler2D AlbedoTex;
layout (binding = 2) uniform sampler2D RoughnessTex;
layout (binding = 3) uniform sampler2D MetallicTex;
layout (binding = 4) uniform sampler2D NormalMap;
layout (binding = 5) uniform sampler2D AmbientOcclusionMap;
layout (binding = 6) uniform sampler2D AlphaTex;

uniform float AlphaDiscard;
uniform mat4 ViewMatrix;
uniform mat4 ModelViewMatrix;

uniform float gammaCorrection;
uniform bool doHDRToneMapping;

const float Pi = 3.14159265358979323846;

uniform struct LightInfo {

    vec4 position;
	vec3 colour;
	float brightness;

    // Spot/Point light only
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;

    // Spotlight only
	vec3 direction;
	float cutoffInner; // Phi
	float cutoffOuter; // Gamma
} lights[4];

struct MaterialInfo {

	vec4 albedo;

	float roughness;
    float metallic;

    float ao;

} material;


// using Trowbridge-Reitz GGX
// tells us how many surface microfacets are aligned to the halfway vector, indicates how smooth the surface is
float normalDistribution(in vec3 n, in vec3 h) {
    
    float a2 = material.roughness * material.roughness * material.roughness * material.roughness;

    float nDotH = max(dot(n, h), 0.0);
//    float nDotH = dot(n, h);

    float d = (nDotH * nDotH) * (a2 - 1.0) + 1.0;

    return a2 / (Pi * (d * d));
}

// using Schlick approximation
// models some light being blocked by microfacet overshadowing / blockage
float geomSchlickGGX(in float dotN) {
    
    float k = ((material.roughness + 1.0) * (material.roughness + 1.0)) / 8.0;

    return dotN / (dotN * (1.0 - k) + k);
}

// using Smith approximation
// tells us how many of the surface microfacets are overshadowed by eachother, causing less reflectivity
float geometryShadowing(in vec3 n, in vec3 v, in vec3 s) {
    
    float ggx1 = geomSchlickGGX(max(dot(n, v), 0.0)); // modelling light reflecting to eye being blocked
    float ggx2 = geomSchlickGGX(max(dot(n, -s), 0.0)); // modelling light from the source being blocked
    
//    float ggx1 = geomSchlickGGX(dot(n, v)); // modelling light reflecting to eye being blocked
//    float ggx2 = geomSchlickGGX(dot(n, -s)); // modelling light from the source being blocked

    return ggx1 * ggx2; // multiply to join models
}

// using Schlick approximation
// tells us how much light is reflected (vs refracted)
vec3 fresnelReflection(in vec3 h, in vec3 v, in vec3 F0) {
    
//    return F0 + (1.0 - F0) * pow(clamp(1.0 - max(dot(h, v), 0.0), 0.0, 1.0), 5.0);
//    return F0 + (1.0 - F0) * pow(1.0 - dot(h, v), 5.0);

    return F0 + (1.0 - F0) * pow(clamp(1.0 - max(dot(h, v), 0.0), 0.0, 1.0), 5.0);
//    return F0 + (1.0 - F0) * pow(1.0 - dot(h, v), 5.0);
}

// Function from https://learnopengl.com/PBR/IBL/Diffuse-irradiance
// 
vec3 fresnelSchlickRoughness(in vec3 h, in vec3 v, in vec3 F0)
{
    return F0 + (max(vec3(1.0 - material.roughness), F0) - F0) * pow(clamp(1.0 - max(dot(h, v), 0.0), 0.0, 1.0), 5.0);
}   

vec3 computeMicrofacetModel(in int lightNo, in vec3 F0, in vec3 n, in vec3 v) {

    
    // --- Prep calculations --- //
    vec4 lPosition = ModelViewMatrix * lights[lightNo].position; // If light position.w == 0, light is a directional light

    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz; // Direction of light ray

//    // skybox reflection vector
//    vec3 r = normalize(mat3(transpose(ViewMatrix)) * reflect(normalize(Position).xyz, Normal)).xyz; // transpose to get the inverse of ViewMatrix
//    vec3 reflection = texture(Skybox, r).rgb; // skybox reflection colour
//    vec3 reflectionColour = (reflection * (1.0 - material.roughness) * material.metallic);// material.albedo.rgb );
////    vec3 reflectionColour = mix((reflection * (1.0 - material.roughness)), material.albedo.rgb, (1.0 - material.metallic));
////    vec3 reflectionColour = (reflection * material.albedo.rgb * (1.0 - material.roughness) * material.metallic);

   
    // --- Attenuation calculation --- //
    float lDistance = length(lPosition - Position) * lPosition.w; // Distance from light

    float attenuation = 1.0 / (1.0 + lights[lightNo].attenuationLinear * lDistance + lights[lightNo].attenuationQuadratic * (lDistance * lDistance));
//    float attenuation = 1.0 / ((lDistance * lDistance) + 1.0);


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
    
//    vec3 radiance = (lights[lightNo].colour * intensity) * attenuation * lights[lightNo].brightness;
    vec3 radiance = (lights[lightNo].colour * lights[lightNo].brightness * intensity) * attenuation;

    // half vector
    vec3 h = normalize(v + -s);

    // Cook-Torrance BRDF
    float D = normalDistribution(n, h);
    float G = geometryShadowing(n, v, s);
    vec3  F = fresnelReflection(h, v, F0);

    // specular
//    vec3 fs = (D * G * F) / (4 * dot(n, v) * dot(n, -s) + 0.0001);
    vec3 fs = (D * G * F) / (4 * max(dot(n, v), 0.0) * max(dot(n, -s), 0.0) + 0.0001);

//    vec3 fs = (D * G * F) * 0.25;
//    vec3 fs = (D * G * F) / (4 * max(dot(n, v), 0.0) * max(dot(n, -s), 0.0) + 0.0001);
//    vec3 fs = (D * G * F) / (4 * dot(n, v) * dot(n, -s) + 0.0001);

    // diffuse - should be called kd
    vec3 fd = (vec3(1.0) - F) * (1.0 - material.metallic);
//    vec3 fd = (vec3(1.0) - F) * (((1.0 - material.metallic) + 0.1) + (reflectionColour * (1.0 - material.roughness)));
//    vec3 fd = ((vec3(1.0) - F) + reflectionColour * (1.0 - material.roughness)) * ((1.0 - material.metallic) + 0.1);// + (reflectionColour * (1.0 - material.roughness)));
//    vec3 fd = mix(albedo, vec3(0.0), material.metallic);
//    vec3 fd = mix(albedo, reflectionColour, material.metallic);
    

//    return ((fd * albedo / Pi) + fs) * radiance * dot(n, -s);
    return (fd * material.albedo.rgb / Pi + fs) * radiance * max(dot(n, -s), 0.0);
//    return ((fd * material.albedo.rgb) / Pi + fs) * radiance * max(dot(n, -s), 0.0);
//    return (((fd * material.albedo.rgb) / Pi) + fs + reflectionColour ) * radiance * max(dot(n, -s), 0.0);
//    return (fd * albedo / Pi + fs) * radiance * max(dot(n, -s), 0.0);

//    return (fd + Pi * fs) * radiance * max(dot(n, -s), 0.0);
//    return ((fd * albedo) + Pi * fs) * radiance * max(dot(n, -s), 0.0);
//    return (((fd * vec3(1.0)) / Pi) + fs) * radiance * max(dot(n, -s), 0.0);
//    return (((fd * material.colour.rgb) / Pi) + fs) * radiance * dot(n, -s);
}


void main() {

    vec3 Lo = vec3(0.0);

    // Get texture pixel
    material.albedo = pow(texture(AlbedoTex, TexCoord), vec4(2.2));
//    vec4 albedoTexColour = pow(texture(AlbedoTex, TexCoord), vec4(2.2));
//    vec4 detailTexColour = texture(DetailTex, TexCoord); // Not proper detail, just secondary albedo
    material.metallic = texture(MetallicTex, TexCoord).r;
    material.roughness = texture(RoughnessTex, TexCoord).r;
    material.ao = texture(AmbientOcclusionMap, TexCoord).r;
    vec4 alphaMap = texture(AlphaTex, TexCoord);
    
    
    // view vector
    vec3 v = normalize(-Position).xyz; 


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
//    vec3 texColour = mix(albedoTexColour.rgb, material.colour.rgb, material.colour.a);
//    vec3 texColour = albedoTexColour.rgb;

    // Calculate surface base reflectivity
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, material.albedo.rgb, material.metallic);
//    F0 = mix(F0, material.albedo.rgb, (1.0 - material.metallic));

    // Compute lighting
    for(int i = 0; i < 4; i++)
    {
        Lo += computeMicrofacetModel(i, F0, n, v);
    }

    
    // skybox reflection vector
    vec3 r = normalize(mat3(transpose(ViewMatrix)) * reflect(normalize(Position).xyz, Normal)).xyz; // transpose to get the inverse of ViewMatrix
    vec3 reflection = texture(Skybox, r).rgb; // skybox reflection colour
    vec3 reflectionColour = (reflection * (1.0 - material.roughness) * material.metallic);// material.albedo.rgb );
//    vec3 reflectionColour = mix((reflection * (1.0 - material.roughness)), material.albedo.rgb, (1.0 - material.metallic));
//    vec3 reflectionColour = (reflection * material.albedo.rgb * (1.0 - material.roughness) * material.metallic);


//    vec3 kd = 1.0 - fresnelSchlickRoughness(n, v, F0);
    vec3 kd = 1.0 - fresnelReflection(n, v, F0);

    vec3 diffuse = (texture(EnvironmentMap, n).rgb + reflectionColour) * material.albedo.rgb;
//    vec3 diffuse = mix(material.albedo.rgb, reflectionColour, material.roughness) * texture(EnvironmentMap, n).rgb;

//    vec3 ambient = (kd * diffuse) * material.ao;
    vec3 ambient = ( diffuse) * material.ao;

    vec3 colour = ambient + Lo;

//    vec3 colour = (vec3(0.03) * material.albedo.rgb * material.ao) + reflectionColour + Lo;
//    vec3 colour = (vec3(0.03) * material.albedo.rgb * material.ao) + Lo;
////    vec3 colour = Lo;

    // HDR tonemapping
    if(doHDRToneMapping)
    {
        colour = colour / (colour + vec3(1.0));
    }

    // Gamma correction
    colour = pow(colour, vec3(1.0/gammaCorrection));


    FragColor = vec4(colour, 1.0);
    
}