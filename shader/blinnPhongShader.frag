#version 460

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;
//in vec3 LightDirection;
//in vec3 ViewDirection;
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
//
//uniform vec4 KColour;
//uniform float KAmbient;
//uniform float KDiffuse;
//uniform float KSpecular;
//uniform float KShininess;
//
//uniform vec4 LightPosition;
//uniform vec3 La;
//uniform vec3 Ld;
//uniform vec3 Ls;
//
//uniform vec3 Ka;
//uniform vec3 Kd;
//uniform vec3 Ks;
//uniform float Shininess;

uniform float AlphaDiscard;

//uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
//uniform mat4 ObjectModelMatrix;
uniform mat4 ModelViewMatrix;
//uniform mat3 NormalMatrix;
//uniform mat4 MVP;
//uniform mat4 ViewProjectionMatrix;

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

    vec4 lPosition = ModelViewMatrix * lights[lightNo].position;

//--    vec3 s = LightDirection;
    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz;

//--    float sDotN = max(dot(s, n), 0.0);
    float sDotN = max(dot(-s, Normal), 0.0);

    float lDistance = length(lPosition - Position) * lPosition.w;

//    float attenuation = 1.0 / (1.0 + 0.09 * lDistance + 0.032 * (lDistance * lDistance)); // for distance 50
//    float attenuation = 1.0 / (1.0 + 0.022 * lDistance + 0.0019 * (lDistance * lDistance)); // for distance 200
    float attenuation = 1.0 / (1.0 + lights[lightNo].attenuationLinear * lDistance + lights[lightNo].attenuationQuadratic * (lDistance * lDistance));

    float intensity = 1.0f;
    
    vec3 lDirection =  (ModelViewMatrix * vec4(lights[lightNo].direction, 0.0)).xyz;

    if((lights[lightNo].direction * lights[lightNo].position.w) != vec3(0.0))
    {
        float theta = dot(s, lDirection);
        float angle = acos(theta);

        float epsilon = lights[lightNo].cutoffInner - lights[lightNo].cutoffOuter;

        intensity = clamp((angle - lights[lightNo].cutoffOuter) / epsilon, 0.0, 1.0);
    }

    colourOut += ((tColour * material.ambient) * (lights[lightNo].colour * lights[lightNo].ambient)) * attenuation;

    colourOut += ((tColour * material.diffuse) * (lights[lightNo].colour * lights[lightNo].diffuse) * sDotN) * intensity * attenuation;


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
    vec3 Ks = tColour * material.specular;
    vec3 Ls = lights[lightNo].colour * lights[lightNo].specular;
    vec3 reflectionColour = texture(Skybox, r).rgb;

    colourOut += (Ks * (Ls * reflectionColour)) * pow( max( dot(h, n), 0.0), material.shininess) * intensity * attenuation;
// 		colourOut += (Ks * (Ls * reflectionColour)) * pow( max( dot(h, n), 0.0), material.shininess);
// 		colourOut += (Ks * mix(reflectionColour, Ls, material.reflectivity)) * pow( max( dot(h, n), 0.0), material.shininess);
// 		colourOut += (Ks * (Ls * (reflectionColour * material.reflectivity))) * pow( max( dot(h, n), 0.0), material.shininess);
    }

    return colourOut;
}


void main() {

//    vec3 Colour = KColour;
    vec3 Colour = vec3(0.0);//KColour;


    vec4 albedoTexColour = texture(AlbedoTex, TexCoord);
    vec4 detailTexColour = texture(DetailTex, TexCoord);
   
    vec3 albedoDetail = normalize(mix(albedoTexColour.rgb, detailTexColour.rgb, detailTexColour.a));

//    vec3 texColour = normalize(mix(albedoDetail.rbg, material.colour.rgb, material.colour.a));
    vec3 texColour = normalize(mix(albedoTexColour.rbg, material.colour.rgb, material.colour.a));

    vec4 alphaMap = texture(AlphaTex, TexCoord);

    vec3 normMap = texture(NormalMap, TexCoord).xyz;


//    normMap.xy = 2.0 * normMap.xy - 1.0;


//    if(alphaMap.a < AlphaDiscard)
//    {
//        discard;
//    }
    
//    if(!gl_FrontFacing)
//    {
//        n = -n;
////        n = -Normal;
//    }

    
//    vec3 texColour = normalize(albedoTexColour).rgb;
////    vec3 texColour = mix(KColour, albedoTexColour.rbg, albedoTexColour.a);
////    vec3 texColour = mix(KColour.rgb, albedoTexColour.rbg, (1.0 -KColour.a));
////    vec3 texColour = normalize(KColour + albedoTexColour).rgb;
////    vec3 texColour = normalize(KColour + albedoTexColour.rbg);
////    vec3 texColour = mix(albedoTexColour.rgb, detailTexColour.rgb, detailTexColour.a);


//
////    vec3 n = normMap;
////    vec3 n = normalize(transpose(TBN_Matrix) * (normMap * 2.0 - 1.0));
    vec3 n = normalize(TBN_Matrix * (normMap * 2.0 - 1.0));
////    n.x = -n.x;
////--*    vec3 n = Normal;
//
//    vec4 lPosition = ModelViewMatrix * LightPosition;
//
////--    vec3 s = LightDirection;
//    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz;
//
////--    float sDotN = max(dot(s, n), 0.0);
//    float sDotN = max(dot(-s, Normal), 0.0);
//
//    Colour += (texColour * KAmbient * La);
//
//    Colour += ((texColour * KDiffuse * Ld) * sDotN);
//
//
//    if(sDotN > 0.0)
//    {
////--        vec3 v = ViewDirection;
//        vec3 v = normalize(-Position).xyz;
//
//
////// original method
////        vec3 r = normalize(mat3(inverse(ViewMatrix)) * reflect(-v, Normal)).xyz;
////// optimised method - the transpose of a rotation matrix is its inverse
////--        vec3 r = normalize(mat3(transpose(ViewMatrix)) * reflect(-v, n)).xyz;
////--*     vec3 r = normalize(mat3(transpose(ViewMatrix)) * reflect(-v, Normal)).xyz;
//     vec3 r = normalize(mat3(transpose(ViewMatrix)) * reflect(normalize(Position).xyz, Normal)).xyz;
//
//
//        vec3 h = normalize(v + -s);
////--*        vec3 h = normalize(v + -s);
//
//        // NOTE TO SELF: using dot(h, v) produces a funky effect - need to investigate
////		Colour += (KSpecular * Ls) * pow( max( dot(h, Normal), 0.0), KShininess);
//
////		Colour += (KSpecular * Ls) * pow( max( dot(h, n), 0.0), KShininess) ;
////--*		Colour += (KSpecular * (Ls * texture(Skybox, r).rgb)) * pow( max( dot(h, Normal), 0.0), KShininess);
// 		Colour += (KSpecular * (Ls * texture(Skybox, r).rgb)) * pow( max( dot(h, n), 0.0), KShininess);
//    }

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

    for(int i = 0; i < 4; i++)
    {
        Colour += computeLighting(i, texColour, n);
    }

    FragColor = vec4(Colour, 1.0);
    
}