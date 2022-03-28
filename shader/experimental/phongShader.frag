#version 460

in vec4 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;

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

void main() {

    vec3 Colour = vec3(0.0);
    
    vec4 lPosition = ModelViewMatrix * LightPosition;

    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz;

    float sDotN = max(dot(-s, Normal), 0.0);

    Colour += Ka * La;

    Colour += (Kd * Ld) * sDotN;

    if(sDotN > 0.0)
    {
        vec3 v = normalize(-Position).xyz;

		vec3 r = reflect(s, Normal);

		Colour += (Ks * Ls) * pow( max( dot(r, v), 0.0), Shininess);
    }

    FragColor = vec4(Colour, 1.0);
}
