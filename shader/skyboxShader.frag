#version 460

//in vec4 vecPosition;
in vec3 TexCoord;
//in vec3 Vec;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 HdrColor;
layout (location = 2) out vec4 BlurOneColor;

layout (binding = 0) uniform samplerCube SkyboxTex;

uniform float LuminanceThreshold = 1.7; // Luminance threshold

float luminance(vec3 colour) {
    
    return 0.2126 * colour.r + 0.7152 * colour.g + 0.0722 * colour.b;
}

void main() {
    
//// Marius code - broke; MVP has been changed to VP matrix due to my Model usage - not that it makes a difference
//    FragColor = vec4(texture(SkyboxTex, normalize(Vec)).rgb, 1.0f);

    vec3 colour = texture(SkyboxTex, TexCoord).rgb;

    float pixelLumen = luminance(colour.rgb);

        if(pixelLumen > LuminanceThreshold) {
    //    if(luminance(colour.rgb) > LuminanceThreshold) {

        //        HdrColor = vec4(0.75, 0.3, 0.68, 1.0);
    //        BlurOneColor = vec4(0.75, 0.3, 0.68, 1.0);
            BlurOneColor = vec4(colour, 1.0);
        }
        else {

                BlurOneColor = vec4(0.0);
        //        BlurOneColor = vec4(0.75, 0.3, 0.68, 1.0);
        //        HdrColor = vec4(0.75, 0.68, 0.3, 1.0);
    //        BlurOneColor = vec4(0.75, 0.68, 0.3, 1.0);
        ////        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        }


    // HDR tonemapping and gamma correction
//    colour = colour / (colour + vec3(1.0));
//    colour = pow(colour, vec3(1.0/2.2));

    // Correct for my Model usage
    HdrColor = vec4(colour, 1.0);
//    FragColor = vec4(colour, 1.0);
}
