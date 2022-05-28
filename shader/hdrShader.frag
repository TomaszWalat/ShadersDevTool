#version 460

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;


// Output buffers (textures)
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 HdrColor;
layout (location = 2) out vec4 BlurOneColor;
layout (location = 3) out vec4 BlurTwoColor;


// --- Textures (for reading) --- //

// Skybox textures
layout (binding = 0) uniform samplerCube Skybox;
layout (binding = 7) uniform samplerCube EnvironmentMap; // Blury - gives a sense of lighting strength

// Object material textures
layout (binding = 1) uniform sampler2D AlbedoTex;
layout (binding = 2) uniform sampler2D RoughnessTex;
layout (binding = 3) uniform sampler2D MetallicTex;
layout (binding = 4) uniform sampler2D NormalMap;
layout (binding = 5) uniform sampler2D AmbientOcclusionMap;
layout (binding = 6) uniform sampler2D AlphaTex;

// HDR and Bloom textures
layout (binding = 8) uniform sampler2D HdrTex;
layout (binding = 9) uniform sampler2D BlurTex1;
layout (binding = 10) uniform sampler2D BlurTex2;


// --- Uniforms --- //

// Render pass number
uniform int PassNo; 

// HDR tone mapping conversion matrices
// XYZ/RGB conversion matrices from:
// http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
uniform mat3 rgb2xyz = mat3(0.4124564, 0.2126729, 0.0193339,
                            0.3575761, 0.7151522, 0.1191920,
                            0.1804375, 0.0721750, 0.9503041);

uniform mat3 xyz2rgb = mat3(3.2404542, -0.9692660, 0.0556434,
                            -1.5371385, 1.8760108, -0.2040259,
                            -0.4985314, 0.0415560, 1.0572252 );

const float Pi = 3.14159265358979323846; // For PBR light calculations

// HDR settings
uniform bool DoHDRToneMapping;
uniform float White = 0.928; // For balancing the colour of HDR lighting
uniform float AverageLumen; // Scene average brightness
uniform float Exposure;

// Bloom settings
uniform bool DoBloom;
uniform float LuminanceThreshold;
uniform int BloomOneStrength;
uniform int BloomTwoStrength;
uniform float PixelOffset[10] = float[](0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
uniform float Weight[10];

uniform float GammaCorrection;

// Camera matrices
uniform mat4 ViewMatrix;
uniform mat4 ModelViewMatrix;

uniform mat4 SkyboxRotationMatrix;

uniform float AlphaDiscard;


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

} lights[4];

struct MaterialInfo {

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
        vec3 lDirection = (ModelViewMatrix * vec4(lights[lightNo].direction, 0.0)).xyz; // Light direction in world space

        float theta = dot(s, lDirection); // angle between light ray (from light to fragment)
        float angle = acos(theta); // real angle

        float epsilon = lights[lightNo].cutoffInner - lights[lightNo].cutoffOuter; // fade out angle

        intensity = clamp((angle - lights[lightNo].cutoffOuter) / epsilon, 0.0, 1.0); // intensity = 1.0 inside the inner cone
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



// Computes shading and stores result in high-res framebuffer (writing to HdrTex)
void passOne() {
    
    vec3 Lo = vec3(0.0);

    // Get textures' pixel data
    material.albedo = pow(texture(AlbedoTex, TexCoord), vec4(2.2)); // Scale for HDR
    material.metallic = texture(MetallicTex, TexCoord).r;
    material.roughness = texture(RoughnessTex, TexCoord).r;
    material.ao = texture(AmbientOcclusionMap, TexCoord).r;
//    vec4 alphaMap = texture(AlphaTex, TexCoord);
    
    // view vector
    vec3 v = normalize(-Position).xyz; 


//    // Discard fragment based on alpah map
//    if(alphaMap.a < AlphaDiscard)
//    {
//        discard;
//    }

    vec3 n = Normal;
//    // Invert face normals if pointing away from camera
//    if(!gl_FrontFacing)
//    {
//        n = -Normal;
//    }


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
    vec3 rView = transpose(mat3(SkyboxRotationMatrix)) * v;
    vec3 rNormal = transpose(mat3(SkyboxRotationMatrix)) * Normal;

    vec3 r = reflect(-rView, rNormal); // transpose to get the inverse of ViewMatrix
    vec3 reflection = texture(Skybox, r).rgb; // skybox reflection colour

    vec3 reflectionColour = (reflection * (1.0 - material.roughness) * material.metallic);

    vec3 environmentLight = texture(EnvironmentMap, mat3(transpose(SkyboxRotationMatrix)) * n).rgb;

    vec3 environmentLighting = environmentLight * reflectionColour * material.albedo.rgb * lights[0].brightness;

    if(DoHDRToneMapping)
    {
        environmentLight *= Exposure; // scale brightness with exposure
    }

    vec3 ambient = environmentLighting * material.ao;

    vec3 colour = ambient + Lo;


    // Bright-pass filter (read from HdrTex, writing to BlurOneColor)
    if(luminance(colour.rgb) > LuminanceThreshold) {

        BlurOneColor = vec4(colour, 1.0);
    }
    else {

        BlurOneColor = vec4(0.0);
    }


    HdrColor = vec4(colour, 1.0);
}



// --- Original blur functions (horizontal then vertical) --- //
// First blur pass (reading from BlurTex1, writing to BlurTex2)
void passTwo() {
    
    float dy = 1.0 / (textureSize(BlurTex1, 0)).y;
    
    int offset = 10 - BloomOneStrength;

    vec4 sum = texture(BlurTex1, TexCoord) * Weight[0 + offset];

    for(int i = 0; i < 10; i++) {
        
        sum += texture(BlurTex1, TexCoord + vec2(0.0, PixelOffset[i]) * dy) * Weight[i + offset];

        sum += texture(BlurTex1, TexCoord - vec2(0.0, PixelOffset[i]) * dy) * Weight[i + offset];
    }

    BlurTwoColor = sum;
}

// Second blur pass (reading from BlurTex2, writing to BlurTex1)
void passThree() {

    float dx = 1.0 / (textureSize(BlurTex2, 0)).x;
    
    int offset = 10 - BloomTwoStrength;

    vec4 sum = texture(BlurTex2, TexCoord) * Weight[0 + offset];

    for(int i = 0; i < BloomTwoStrength; i++) {
        
        sum += texture(BlurTex2, TexCoord + vec2(PixelOffset[i], 0.0) * dx) * Weight[i + offset];

        sum += texture(BlurTex2, TexCoord - vec2(PixelOffset[i], 0.0) * dx) * Weight[i + offset];
    }

    BlurOneColor = sum;
}


// --- Version 2 blur functions (horizontal + vertical, repeated) --- //
// First blur pass (reading from BlurTex1, writing to BlurTex2)
void passTwo_v2() {
    
    float dy = 1.0 / (textureSize(BlurTex1, 0)).y;
    float dx = 1.0 / (textureSize(BlurTex1, 0)).x;
    
    int offset = 10 - BloomOneStrength;

    vec4 sum = texture(BlurTex1, TexCoord) * Weight[0 + offset];

    for(int i = 0; i < BloomOneStrength; i++) {
        
        sum += texture(BlurTex1, TexCoord + vec2(0.0, PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex1, TexCoord + vec2(PixelOffset[i] * dx, 0.0)) * Weight[i + offset];

        sum += texture(BlurTex1, TexCoord - vec2(0.0, PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex1, TexCoord - vec2(PixelOffset[i] * dx, 0.0)) * Weight[i + offset];
    }

    BlurTwoColor = sum;
}

// Second blur pass (reading from BlurTex2, writing to BlurTex1)
void passThree_v2() {
    
    
    float dy = 1.0 / (textureSize(BlurTex2, 0)).y;
    float dx = 1.0 / (textureSize(BlurTex2, 0)).x;
    
    int offset = 10 - BloomTwoStrength;

    vec4 sum = texture(BlurTex2, TexCoord) * Weight[0 + offset];

    for(int i = 0; i < BloomTwoStrength; i++) {
        
        sum += texture(BlurTex2, TexCoord + vec2(0.0, PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex2, TexCoord + vec2(PixelOffset[i] * dx, 0.0)) * Weight[i + offset];

        sum += texture(BlurTex2, TexCoord - vec2(0.0, PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex2, TexCoord - vec2(PixelOffset[i] * dx, 0.0)) * Weight[i + offset];
    }

    BlurOneColor = sum;
}


// --- Version 3 blur functions (horizontal + vertical, then diagonal) --- //
// First blur pass (reading from BlurTex1, writing to BlurTex2)
void passTwo_v3() {
    
    float dy = 1.0 / (textureSize(BlurTex1, 0)).y;
    float dx = 1.0 / (textureSize(BlurTex1, 0)).x;
    
    int offset = 10 - BloomOneStrength;

    vec4 sum = texture(BlurTex1, TexCoord) * Weight[0 + offset];

    for(int i = 0; i < BloomOneStrength; i++) {
        
        sum += texture(BlurTex1, TexCoord + vec2(0.0, PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex1, TexCoord + vec2(PixelOffset[i] * dx, 0.0)) * Weight[i + offset];

        sum += texture(BlurTex1, TexCoord - vec2(0.0, PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex1, TexCoord - vec2(PixelOffset[i] * dx, 0.0)) * Weight[i + offset];
    }

    BlurTwoColor = sum;
}

// Second blur pass (reading from BlurTex2, writing to BlurTex1)
void passThree_v3() {
    
    float dy = 1.0 / (textureSize(BlurTex2, 0)).y;
    float dx = 1.0 / (textureSize(BlurTex2, 0)).x;
    
    int offset = 10 - BloomTwoStrength;

    vec4 sum = texture(BlurTex2, TexCoord) * Weight[0 + offset];

    for(int i = 0; i < BloomTwoStrength; i++) {
        
        sum += texture(BlurTex2, TexCoord + vec2( PixelOffset[i] * dx,  PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex2, TexCoord + vec2( PixelOffset[i] * dx, -PixelOffset[i] * dy)) * Weight[i + offset];

        sum += texture(BlurTex2, TexCoord + vec2(-PixelOffset[i] * dx, -PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex2, TexCoord + vec2(-PixelOffset[i] * dx,  PixelOffset[i] * dy)) * Weight[i + offset];
    }

    BlurOneColor = sum;
}


// --- Version 4 blur functions (horizontal + vertical + diagonal, repeated) --- //
// First blur pass (reading from BlurTex1, writing to BlurTex2)
void passTwo_v4() {
    
    float dy = 1.0 / (textureSize(BlurTex1, 0)).y;
    float dx = 1.0 / (textureSize(BlurTex1, 0)).x;
    
    int offset = 10 - BloomOneStrength;

    vec4 sum = texture(BlurTex1, TexCoord) * Weight[0 + offset];

    for(int i = 0; i < BloomOneStrength; i++) {
        
        sum += texture(BlurTex1, TexCoord + vec2(0.0, PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex1, TexCoord + vec2(PixelOffset[i] * dx, 0.0)) * Weight[i + offset];

        sum += texture(BlurTex1, TexCoord - vec2(0.0, PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex1, TexCoord - vec2(PixelOffset[i] * dx, 0.0)) * Weight[i + offset];


        sum += texture(BlurTex1, TexCoord + vec2( PixelOffset[i] * dx,  PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex1, TexCoord + vec2( PixelOffset[i] * dx, -PixelOffset[i] * dy)) * Weight[i + offset];

        sum += texture(BlurTex1, TexCoord + vec2(-PixelOffset[i] * dx, -PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex1, TexCoord + vec2(-PixelOffset[i] * dx,  PixelOffset[i] * dy)) * Weight[i + offset];
    }

    BlurTwoColor = sum;
}

// Second blur pass (reading from BlurTex2, writing to BlurTex1)
void passThree_v4() {
    
    float dy = 1.0 / (textureSize(BlurTex2, 0)).y;
    float dx = 1.0 / (textureSize(BlurTex2, 0)).x;

    int offset = 10 - BloomTwoStrength;

    vec4 sum = texture(BlurTex2, TexCoord) * Weight[0 + offset];

    for(int i = 0; i < BloomTwoStrength; i++) {

        sum += texture(BlurTex2, TexCoord + vec2(0.0, PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex2, TexCoord + vec2(PixelOffset[i] * dx, 0.0)) * Weight[i + offset];

        sum += texture(BlurTex2, TexCoord - vec2(0.0, PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex2, TexCoord - vec2(PixelOffset[i] * dx, 0.0)) * Weight[i + offset];

        
        sum += texture(BlurTex2, TexCoord + vec2( PixelOffset[i] * dx,  PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex2, TexCoord + vec2( PixelOffset[i] * dx, -PixelOffset[i] * dy)) * Weight[i + offset];

        sum += texture(BlurTex2, TexCoord + vec2(-PixelOffset[i] * dx, -PixelOffset[i] * dy)) * Weight[i + offset];
        sum += texture(BlurTex2, TexCoord + vec2(-PixelOffset[i] * dx,  PixelOffset[i] * dy)) * Weight[i + offset];
    }

    BlurOneColor = sum;
}



// Apply HDR tone mapping to HdrTex, then combine with blured bright-pass filter
// (reading from HdrTex & BlurTex1, writing to default buffer)
void passFour() {

    // --- Tone mapping --- //

    // Retrive the colour from high-res texture
    vec4 colour = texture(HdrTex, TexCoord);


    // Convert to XYZ - LDR [0.0, 1.0] range to HDR [0.0, float max value] range
    vec3 xyzColour = rgb2xyz * vec3(colour);

    // Convert to xyY
    float xyzSum = xyzColour.x + xyzColour.y + xyzColour.z;
    vec3 xyYColour = vec3(xyzColour.x / xyzSum, xyzColour.y / xyzSum, xyzColour.y);

    // Tone map the luminance - use xyYColour.z or xyzColour.y (they're the same value)
    float L = (Exposure * xyYColour.z) / AverageLumen;
    L = (L * (1 + L / (White * White))) / (1 + L);

    // Convert back to XYZ using toned luminance
    xyzColour.x = (L * xyYColour.x) / xyYColour.y;
    xyzColour.y = L;
    xyzColour.z = (L * (1 - xyYColour.x - xyYColour.y)) / xyYColour.y;


    if(DoHDRToneMapping) {
        // Convert back to RGB
        colour = vec4(xyz2rgb * xyzColour, 1.0);
    }


    // --- Combine HDR with blurred texture --- //
    if(DoBloom)
    {
        vec4 blurredTex = texture(BlurTex1, TexCoord); // accessing with linear filtering gives us extra blur
        colour += blurredTex;
    }


    // Gamma correction
    colour = pow(colour, vec4(1.0 / GammaCorrection));


    FragColor = colour;
//    FragColor = texture(BlurTex1, TexCoord);
//    FragColor = texture(BlurTex2, TexCoord);
}



void main() {
    
    if (PassNo == 1) {
        passOne();
    }
    else if (PassNo == 2) {
//        passTwo();
//        passTwo_v2();
//        passTwo_v3();
        passTwo_v4();
    }
    else if (PassNo == 3) {
//        passThree();
//        passThree_v2();
//        passThree_v3();
        passThree_v4();
    }
    else if (PassNo == 4) {
        passFour();
    }
}