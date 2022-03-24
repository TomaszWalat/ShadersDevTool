#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

flat out vec3 Colour;

uniform vec4 LightPosition;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Shininess;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
    Colour = vec3(0.0);
    
    vec4 vPosition = ModelViewMatrix * vec4(VertexPosition, 1.0);

    vec4 lPosition = ModelViewMatrix * LightPosition;

    vec3 n = normalize(NormalMatrix * VertexNormal);
    
    vec3 s = normalize((vPosition * lPosition.w) - lPosition).xyz;

    float sDotN = max(dot(-s, n), 0.0);

    Colour += Ka * La;

    Colour += (Kd * Ld) * sDotN;

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
