#version 460

in vec4 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 HdrColor;
layout (location = 2) out vec4 BlurOneColor;

uniform mat4 ObjectModelMatrix;
uniform mat4 ModelViewMatrix;

uniform float LightType;

uniform vec3 LightColour;
uniform float Brightness;

// Spotlight only
uniform vec3 Direction;
uniform float CutoffInner; // Phi
uniform float CutoffOuter; // Gamma

uniform float LuminanceThreshold = 1.7; // Luminance threshold

void main() {

    vec3 colour = LightColour;// * Brightness;

    // --- Prep calculations --- //
    vec4 lPosition = ModelViewMatrix * ObjectModelMatrix[3]; // If light position.w == 0, light is a directional light

    vec3 s = normalize((Position * LightType) - lPosition).xyz; // Direction of light ray


    // --- Spotlight calculation --- //
    float intensity = 1.0f;
    
    vec3 l = Direction * LightType; // direction / point light filter

    if(l != vec3(0.0)) // if light direcction or position.w == 0, light is not a spotlight
    {
        vec3 lDirection = (ModelViewMatrix * normalize(vec4(Direction, 0.0))).xyz; // Light direction in world space

        float theta = dot(s, lDirection); // angle between light ray (from light to fragment)

        float epsilon = CutoffOuter - CutoffInner; // fade out angle

        intensity = clamp((theta - CutoffOuter) / epsilon, 0.0, 1.0); // intensity = 1.0 inside the inner cone
    }
    

    vec3 radiance = LightColour * Brightness * intensity;// * attenuation;

    colour *= radiance;

    BlurOneColor = vec4(colour, 1.0);
        

    HdrColor = vec4(colour, 1.0);
}
