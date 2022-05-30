#version 460

in vec3 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 HdrColor;
layout (location = 2) out vec4 BlurOneColor;

layout (binding = 0) uniform samplerCube SkyboxTex;

uniform float LuminanceThreshold = 1.7; // Luminance threshold

float luminance(vec3 colour) {
    
    return 0.2126 * colour.r + 0.7152 * colour.g + 0.0722 * colour.b;
}

void main() {

    vec3 colour = texture(SkyboxTex, TexCoord).rgb;

    if(luminance(colour.rgb) > LuminanceThreshold) {

        BlurOneColor = vec4(colour, 1.0);
    }
    else {

        BlurOneColor = vec4(0.0);
    }

    HdrColor = vec4(colour, 1.0);
}
