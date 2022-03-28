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

    float cutOff; // = 0 < f < 1.57 (pi/2)
};

void main() {


    vec3 Colour = vec3(0.0);
    
    vec4 lPosition = ModelViewMatrix * LightPosition;

    vec3 s = normalize((Position * lPosition.w) - lPosition).xyz;

    float sDotN = max(dot(-s, Normal), 0.0);

    float d = length(lPosition - Position) * lPosition.w;

//    float attenuation = 1.0 / (1.0 + 0.09 * d + 0.032 * (d * d));
    float attenuation = 1.0 / (1.0 + 0.022 * d + 0.0019 * (d * d));

    vec4 lightDirection = ModelViewMatrix * vec4(0.0, -1.0, 0.0, 0.0);

    float cosAng = dot(s, lightDirection.xyz); // theta
    float angle = acos(cosAng);

    // cutOff - outerCutOff -> reversing these along with invverting lightDirection above gives an intersting result
    float epsilon = 0.785 - 0.3925; 

    // swap these around for the above experiment
//    float intensity = clamp((cosAng - 0.785) / epsilon, 0.0, 1.0);
    float intensity = clamp((cosAng - 0.3925) / epsilon, 0.0, 1.0);

    Colour += (Ka * La) * attenuation;

    float spotScale = pow(cosAng, 16.0);
    Colour += ((Kd * Ld) * sDotN) * intensity * attenuation;

    if(sDotN > 0.0)
    {
        vec3 v = normalize(-Position).xyz;

        vec3 h = normalize(v + -s);

        // NOTE TO SELF: using dot(h, v) produces a funky effect - need to investigate
		Colour += ((Ks * Ls) * pow( max( dot(h, Normal), 0.0), Shininess)) * intensity * attenuation;
    }

//    if(angle < 0.1)//0.785)
//    {
//        float spotScale = pow(cosAng, 16.0);
//        Colour += ((Kd * Ld) * sDotN) * spotScale * attenuation;
//
//        if(sDotN > 0.0)
//        {
//            vec3 v = normalize(-Position).xyz;
//
//            vec3 h = normalize(v + -s);
//
//            // NOTE TO SELF: using dot(h, v) produces a funky effect - need to investigate
//		    Colour += ((Ks * Ls) * pow( max( dot(h, Normal), 0.0), Shininess)) * spotScale * attenuation;
//        }
//    }

    FragColor = vec4(Colour, 1.0);
}
