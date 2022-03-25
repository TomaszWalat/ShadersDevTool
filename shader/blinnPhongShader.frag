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

struct LightInfo {
    vec4 Position;
    vec3 Direction;

    vec3 Colour;
    float aIntensity;
    float dIntensity;
    float sIntensity;

    float atConstant;   // = 1.0
    float atLinear;     // = 0.09
    float atQuadratic;  // = 0.032

    float cutOff;
};

void main() {


    vec3 Colour = vec3(0.0);
    
    vec4 lPosition = ModelViewMatrix * LightPosition;

    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz;

    float sDotN = max(dot(-s, Normal), 0.0);

    float d = length(lPosition - Position) * lPosition.w;

//    float attenuation = 1.0 / (1.0 + 0.09 * d + 0.032 * (d * d));
    float attenuation = 1.0 / (1.0 + 0.022 * d + 0.0019 * (d * d));

    Colour += (Ka * La) * attenuation;

    Colour += ((Kd * Ld) * sDotN) * attenuation;

    if(sDotN > 0.0)
    {
        vec3 v = normalize(-Position).xyz;

        vec3 h = normalize(v + -s);

        // NOTE TO SELF: using dot(h, v) produces a funky effect - need to investigate
		Colour += ((Ks * Ls) * pow( max( dot(h, Normal), 0.0), Shininess)) * attenuation;
    }

    FragColor = vec4(Colour, 1.0);
}
