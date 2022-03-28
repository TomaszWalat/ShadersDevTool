#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

out vec4 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec3 Tangent;
out vec3 Binormal;
//out vec3 LightDirection;
//out vec3 ViewDirection;
out mat3 TBN_Matrix;


//uniform vec4 LightPosition;
//uniform mat4 ModelMatrix;
//uniform mat4 ViewMatrix;
uniform mat4 ObjectModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
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

void computeNormalSpace(){

	Normal = normalize(NormalMatrix * VertexNormal);

	Tangent = normalize(NormalMatrix * vec3(VertexTangent));
//    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);

    Binormal = normalize(cross(Normal, Tangent)) * VertexTangent.w;

    TBN_Matrix = mat3(Tangent, Binormal, Normal);

//    TBN_Matrix = mat3(Tangent.x, Binormal.x, Normal.x, 
//                      Tangent.y, Binormal.y, Normal.y,
//                      Tangent.z, Binormal.z, Normal.z);
}

void main()
{
	computeNormalSpace();

    Position = ModelViewMatrix * ObjectModelMatrix * vec4(VertexPosition, 1.0);

    TexCoord = VertexTexCoord;

    gl_Position = MVP * ObjectModelMatrix * vec4(VertexPosition, 1.0);

    
//    vec4 lPos = ModelViewMatrix * LightPosition;
//
////    LightDirection = TBN_Matrix * normalize((Position * lPos.w) - lPos).xyz;
//    LightDirection = TBN_Matrix * normalize(lPos).xyz;
////    LightDirection = toObjLocal * (LightPosition - Position).xyz;
//    ViewDirection = TBN_Matrix * normalize(Position).xyz;

}

