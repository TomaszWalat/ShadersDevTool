#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

out vec4 Position;
out vec3 Normal;
out vec2 TexCoord;
out mat3 TBN_Matrix;

uniform mat4 ObjectModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void computeNormalSpace(){

	Normal = normalize(NormalMatrix * VertexNormal);

	vec3 Tangent = normalize(NormalMatrix * vec3(VertexTangent));

    vec3 Binormal = normalize(cross(Normal, Tangent)) * VertexTangent.w;

    TBN_Matrix = mat3(Tangent, Binormal, Normal);
}

void main()
{
	computeNormalSpace();

    Position = ModelViewMatrix * ObjectModelMatrix * vec4(VertexPosition, 1.0);

    TexCoord = VertexTexCoord;

    gl_Position = MVP * ObjectModelMatrix * vec4(VertexPosition, 1.0);
}

