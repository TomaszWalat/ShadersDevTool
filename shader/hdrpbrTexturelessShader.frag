#version 460

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;


// Output buffers (textures)
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 HdrColor;
layout (location = 2) out vec4 BlurOneColor;


// --- Textures (for reading) --- //

// Skybox textures
layout (binding = 0) uniform samplerCube Skybox;
layout (binding = 7) uniform samplerCube EnvironmentMap; // Blury - gives a sense of lighting strength


// --- Uniforms --- //
const float Pi = 3.14159265358979323846; // For PBR light calculations

// HDR settings
uniform bool DoHDRToneMapping;
uniform float Exposure;

// Bloom settings
uniform float LuminanceThreshold;

uniform float GammaCorrection;

// Camera matrices
uniform mat4 ViewMatrix;
uniform mat4 ModelViewMatrix;

uniform mat4 SkyboxRotationMatrix;

//uniform float AlphaDiscard;


// --- Structs --- //

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

} lights[6];

uniform struct MaterialInfo {

	vec4 albedo; // Colour
	float roughness;
    float metallic;
    float ao; // Ambient Occlussion

} material;



// using Trowbridge-Reitz GGX
// tells us how many surface microfacets are aligned to the halfway vector, indicates how smooth the surface is
float normalDistribution(in vec3 n, in vec3 h) {
    
    float a2 = material.roughness * material.roughness * material.roughness * material.roughness;

    float nDotH = max(dot(n, h), 0.0);

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
    
    return ggx1 * ggx2; // multiply to join models
}


// using Schlick approximation
// tells us how much light is reflected (vs refracted)
vec3 fresnelReflection(in vec3 h, in vec3 v, in vec3 F0) {
    
    return F0 + (1.0 - F0) * pow(clamp(1.0 - max(dot(h, v), 0.0), 0.0, 1.0), 5.0);
}


// PBR light calculation
vec3 computeMicrofacetModel(in int lightNo, in vec3 F0, in vec3 n, in vec3 v) {

    // --- Prep calculations --- //
    vec4 lPosition = ModelViewMatrix * lights[lightNo].position; // If light position.w == 0, light is a directional light

    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz; // Direction of light ray

   
    // --- Attenuation calculation --- //
    float lDistance = length(lPosition - Position) * lPosition.w; // Distance from light

    float attenuation = 1.0 / (1.0 + lights[lightNo].attenuationLinear * lDistance + lights[lightNo].attenuationQuadratic * (lDistance * lDistance));


    // --- Spotlight calculation --- //
    float intensity = 1.0f;
    
    vec3 l = lights[lightNo].direction * lights[lightNo].position.w; // direction / point light filter

    if(l != vec3(0.0)) // if light direcction or position.w == 0, light is not a spotlight
    {
        vec3 lDirection = (ModelViewMatrix * normalize(vec4(lights[lightNo].direction, 0.0))).xyz; // Light direction in world space

        float theta = dot(s, lDirection); // angle between light ray (from light to fragment)

        float epsilon = lights[lightNo].cutoffOuter - lights[lightNo].cutoffInner; // fade out angle
//        float epsilon = lights[lightNo].cutoffInner - lights[lightNo].cutoffOuter; // fade out angle

        intensity = clamp((theta - lights[lightNo].cutoffOuter) / epsilon, 0.0, 1.0); // intensity = 1.0 inside the inner cone
    }
    
    vec3 radiance = lights[lightNo].colour * lights[lightNo].brightness * intensity * attenuation;

    // half vector
    vec3 h = normalize(v + -s);

    // Cook-Torrance BRDF
    float D = normalDistribution(n, h);
    float G = geometryShadowing(n, v, s);
    vec3  F = fresnelReflection(h, v, F0);

    // specular
    vec3 specular = (D * G * F) / (4 * max(dot(n, v), 0.0) * max(dot(n, -s), 0.0) + 0.0001);

    // refracted light
    vec3 kd = (vec3(1.0) - F) * (1.0 - material.metallic);
    
    return ((kd * material.albedo.rgb) / Pi + specular) * radiance * max(dot(n, -s), 0.0);
}


// Computes colour luminance
float luminance(vec3 colour) {
    
    return 0.2126 * colour.r + 0.7152 * colour.g + 0.0722 * colour.b;
}



// Computes shading and stores result in high-res framebuffer (writing to HDR tex and Blur one tex)
void main() {
    
    vec3 Lo = vec3(0.0);

    // view vector
    vec3 v = normalize(-Position).xyz; 

    vec3 n = normalize(Normal);

    // Calculate surface base reflectivity
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, material.albedo.rgb, material.metallic);
//    F0 = mix(F0, material.albedo.rgb, (1.0 - material.metallic));

    // Compute lighting
    for(int i = 0; i < 6; i++)
    {
        Lo += computeMicrofacetModel(i, F0, n, v);
    }

    // skybox reflection vector - transpose to get the inverse of SkyboxRotationMatrix (which has ViewMatrix within it)
    vec3 rView = transpose(mat3(SkyboxRotationMatrix)) * v;
    vec3 rNormal = transpose(mat3(SkyboxRotationMatrix)) * Normal;

    vec3 r = reflect(-rView, rNormal); 
    vec3 reflection = texture(Skybox, r).rgb; // skybox reflection colour

    vec3 environment = texture(EnvironmentMap, mat3(transpose(SkyboxRotationMatrix)) * n).rgb; // environment lighting


    // ambient lighting - based on environment (skybox)
    vec3 kS = fresnelReflection(n, v, F0); // light reflected (percentage)

    vec3 kD = (1.0 - kS) * (1.0 - material.metallic); // light refracted (percentage). Absorbed by metals

    vec3 irradiance = environment * material.albedo.rgb; // combines skybox brightness with base colour

    vec3 ambient = (kD * irradiance) * material.ao; // ambient lighting

    vec3 reflectionColour = (reflection * material.albedo.rgb) * (1.0 - material.roughness); // calculate strength of reflection

    vec3 result = ambient + mix(reflectionColour, material.albedo.rgb, (1.0 - material.metallic)); // combine ambient + environment
//    vec3 result = mix(reflectionColour, material.albedo.rgb, (1.0 - material.metallic)); // combine ambient + environment

    // scale environment lighting with exposure and "scene" brightness (using the main light is temporary)
    if(DoHDRToneMapping)
    {
        result *= Exposure  * lights[0].brightness; // scale brightness with exposure
    }

    vec3 colour = result + Lo;

    // Bright-pass filter
    if(luminance(colour.rgb) > LuminanceThreshold) {

        BlurOneColor = vec4(colour, 1.0);
    }
    else {

        BlurOneColor = vec4(0.0);
    }


//    HdrColor = vec4(1.0);
    HdrColor = vec4(colour, 1.0);
}