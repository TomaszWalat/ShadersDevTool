#version 460

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform samplerCube Skybox;

layout (binding = 1) uniform sampler2D AlbedoTex;
layout (binding = 2) uniform sampler2D DetailTex;
//layout (binding = 2) uniform sampler2D AlphaTex;
//layout (binding = 3) uniform sampler2D NormalMap;
//layout (binding = 4) uniform sampler2D DisplacementMap;
//layout (binding = 5) uniform sampler2D AmbientOcclusionMap;
//layout (binding = 6) uniform sampler2D SpecularMap;

//layout (binding = 1) uniform Material
//{
//    vec3 Colour;
//    float Ambient;
//    float Diffuse;
//    float Specular;
//    float Shininess_;
//};

uniform vec3 KColour;
uniform float KAmbient;
uniform float KDiffuse;
uniform float KSpecular;
uniform float KShininess;

uniform vec4 LightPosition;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Shininess;

//uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
//uniform mat4 ObjectModelMatrix;
uniform mat4 ModelViewMatrix;
//uniform mat3 NormalMatrix;
//uniform mat4 MVP;
//uniform mat4 ViewProjectionMatrix;

void main() {

    vec3 Colour = vec3(0.0);//KColour;
    
    vec4 albedoTexColour = texture(AlbedoTex, TexCoord);
    vec4 detailTexColour = texture(DetailTex, TexCoord);

    vec3 texColour = mix(albedoTexColour.rgb, detailTexColour.rgb, detailTexColour.a);

    vec4 lPosition = ModelViewMatrix * LightPosition;

    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz;

    float sDotN = max(dot(-s, Normal), 0.0);

    Colour += (texColour * KAmbient * La);

    Colour += ((texColour * KDiffuse * Ld) * sDotN);


    if(sDotN > 0.0)
    {
        vec3 v = normalize(-Position).xyz;


//// original method
//        vec3 r = normalize(mat3(inverse(ViewMatrix)) * reflect(-v, Normal)).xyz;
//// optimised method
        vec3 r = normalize(mat3(transpose(ViewMatrix)) * reflect(-v, Normal)).xyz;


        vec3 h = normalize(v + -s);

        // NOTE TO SELF: using dot(h, v) produces a funky effect - need to investigate
//		Colour += (KSpecular * Ls) * pow( max( dot(h, Normal), 0.0), KShininess);

		Colour += (KSpecular * (Ls * texture(Skybox, r).rgb)) * pow( max( dot(h, Normal), 0.0), KShininess) ;
    }

//    vec3 texColour = texture(AlbedoTex, TexCoord).rgb;
//
//    vec4 lPosition = ModelViewMatrix * LightPosition;
//
//    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz;
//
//    float sDotN = max(dot(-s, Normal), 0.0);
//
//    Colour += (texColour * Ka * La);
//
//    Colour += ((texColour * Kd * Ld) * sDotN);
//
//    if(sDotN > 0.0)
//    {
//        vec3 v = normalize(-Position).xyz;
//
//        vec3 h = normalize(v + -s);
//
//        // NOTE TO SELF: using dot(h, v) produces a funky effect - need to investigate
//		Colour += (Ks * Ls) * pow( max( dot(h, Normal), 0.0), Shininess);
//    }

    FragColor = vec4(Colour, 1.0);
}
