#version 460

//in vec4 vecPosition;
in vec3 TexCoord;
//in vec3 Vec;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform samplerCube SkyboxTex;

void main() {
    
//// Marius code - broke; MVP has been changed to VP matrix due to my Model usage - not that it makes a difference
//    FragColor = vec4(texture(SkyboxTex, normalize(Vec)).rgb, 1.0f);

// Correct for my Model usage
    FragColor = texture(SkyboxTex, TexCoord);
}
