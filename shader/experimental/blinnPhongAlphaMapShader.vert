#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec4 Position;
out vec3 Normal;
out vec2 TexCoord;

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

    gl_Position = MVP * ObjectModelMatrix * vec4(VertexPosition, 1.0);
}
