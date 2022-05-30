#version 460

layout (location = 0) in vec3 VertexPosition;

out vec3 TexCoord;

uniform mat4 ViewProjectionMatrix;

void main()
{

    TexCoord = VertexPosition;
    gl_Position = (ViewProjectionMatrix * vec4(VertexPosition, 1.0));
}
