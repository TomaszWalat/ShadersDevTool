#version 460

//in vec4 vecPosition;
in vec3 TexCoord;
//in vec3 Vec;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform samplerCube SkyboxTex;

void main() {
    
//// Marius code - broke; MVP has been changed to VP matrix due to my Model usage - not that it makes a difference
//    FragColor = vec4(texture(SkyboxTex, normalize(Vec)).rgb, 1.0f);

    vec3 colour = texture(SkyboxTex, TexCoord).rgb;
//
//    // HDR tonemapping and gamma correction
    colour = colour / (colour + vec3(1.0));
//    colour = pow(colour, vec3(1.0/2.2));

    // Correct for my Model usage
    FragColor = vec4(colour, 1.0);
}
