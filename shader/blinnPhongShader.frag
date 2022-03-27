#version 460

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 LightDirection;
in vec3 ViewDirection;
in mat3 TBN_Matrix;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform samplerCube Skybox;

layout (binding = 1) uniform sampler2D AlbedoTex;
layout (binding = 2) uniform sampler2D DetailTex;
layout (binding = 3) uniform sampler2D AlphaTex;
layout (binding = 4) uniform sampler2D NormalMap;
//layout (binding = 5) uniform sampler2D DisplacementMap;
//layout (binding = 6) uniform sampler2D AmbientOcclusionMap;
//layout (binding = 7) uniform sampler2D SpecularMap;

//layout (binding = 1) uniform Material
//{
//    vec3 Colour;
//    float Ambient;
//    float Diffuse;
//    float Specular;
//    float Shininess_;
//};

uniform vec4 KColour;
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

uniform float AlphaDiscard;

//uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
//uniform mat4 ObjectModelMatrix;
uniform mat4 ModelViewMatrix;
//uniform mat3 NormalMatrix;
//uniform mat4 MVP;
//uniform mat4 ViewProjectionMatrix;

void main() {

    vec4 alphaMap = texture(AlphaTex, TexCoord);

    vec3 normMap = texture(NormalMap, TexCoord).xyz;

    
//    normMap.xy = 2.0 * normMap.xy - 1.0;

//    vec3 n = normMap;
    vec3 n = normalize(transpose(TBN_Matrix) * (normMap * 2.0 - 1.0));
//    n.x = -n.x;
//--*    vec3 n = Normal;

//    if(alphaMap.a < AlphaDiscard)
//    {
//        discard;
//    }
    
//    if(!gl_FrontFacing)
//    {
//        n = -n;
////        n = -Normal;
//    }

//    vec3 Colour = KColour;
    vec3 Colour = vec3(0.0);//KColour;
    
    vec4 albedoTexColour = texture(AlbedoTex, TexCoord);
    vec4 detailTexColour = texture(DetailTex, TexCoord);
    vec3 albedoDetail = normalize(mix(albedoTexColour.rgb, detailTexColour.rgb, detailTexColour.a));
//    vec3 texColour = normalize(albedoTexColour).rgb;
////    vec3 texColour = mix(KColour, albedoTexColour.rbg, albedoTexColour.a);
////    vec3 texColour = mix(KColour.rgb, albedoTexColour.rbg, (1.0 -KColour.a));
    vec3 texColour = normalize(mix(albedoTexColour.rbg, KColour.rgb, KColour.a));
////    vec3 texColour = normalize(KColour + albedoTexColour).rgb;
////    vec3 texColour = normalize(KColour + albedoTexColour.rbg);
////    vec3 texColour = mix(albedoTexColour.rgb, detailTexColour.rgb, detailTexColour.a);

    vec4 lPosition = ModelViewMatrix * LightPosition;

//--    vec3 s = LightDirection;
    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz;

//--    float sDotN = max(dot(s, n), 0.0);
    float sDotN = max(dot(-s, Normal), 0.0);

    Colour += (texColour * KAmbient * La);

    Colour += ((texColour * KDiffuse * Ld) * sDotN);


    if(sDotN > 0.0)
    {
//--        vec3 v = ViewDirection;
        vec3 v = normalize(-Position).xyz;


//// original method
//        vec3 r = normalize(mat3(inverse(ViewMatrix)) * reflect(-v, Normal)).xyz;
//// optimised method - the transpose of a rotation matrix is its inverse
//--        vec3 r = normalize(mat3(transpose(ViewMatrix)) * reflect(-v, n)).xyz;
//--*     vec3 r = normalize(mat3(transpose(ViewMatrix)) * reflect(-v, Normal)).xyz;
     vec3 r = normalize(mat3(transpose(ViewMatrix)) * reflect(normalize(Position).xyz, Normal)).xyz;


        vec3 h = normalize(v + -s);
//--*        vec3 h = normalize(v + -s);

        // NOTE TO SELF: using dot(h, v) produces a funky effect - need to investigate
//		Colour += (KSpecular * Ls) * pow( max( dot(h, Normal), 0.0), KShininess);

//		Colour += (KSpecular * Ls) * pow( max( dot(h, n), 0.0), KShininess) ;
//--*		Colour += (KSpecular * (Ls * texture(Skybox, r).rgb)) * pow( max( dot(h, Normal), 0.0), KShininess);
 		Colour += (KSpecular * (Ls * texture(Skybox, r).rgb)) * pow( max( dot(h, n), 0.0), KShininess);
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
