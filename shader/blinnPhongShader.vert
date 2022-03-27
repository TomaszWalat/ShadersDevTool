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
out vec3 LightDirection;
out vec3 ViewDirection;


uniform vec4 LightPosition;
//uniform mat4 ModelMatrix;
//uniform mat4 ViewMatrix;
uniform mat4 ObjectModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
//uniform mat4 ViewProjectionMatrix;

void main()
{
    Position = ModelViewMatrix * ObjectModelMatrix * vec4(VertexPosition, 1.0);

    Normal = normalize(NormalMatrix * VertexNormal);

    TexCoord = VertexTexCoord;

    Tangent = normalize(NormalMatrix * vec3(VertexTangent));

    Binormal = normalize(cross(Normal, Tangent)) * VertexTangent.w;

    mat3 toObjLocal = mat3(Tangent.x, Binormal.x, Normal.x, 
                           Tangent.y, Binormal.y, Normal.y,
                           Tangent.z, Binormal.z, Normal.z);

    vec4 lPosition = ModelViewMatrix * LightPosition;
    LightDirection = toObjLocal * normalize((Position * lPosition.w) - lPosition).xyz;
//    LightDirection = toObjLocal * (LightPosition - Position).xyz;
    ViewDirection = toObjLocal * normalize(-Position).xyz;

    gl_Position = MVP * ObjectModelMatrix * vec4(VertexPosition, 1.0);
}
