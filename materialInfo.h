#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct MaterialInfo
{
	// For materials not dictated by textures
	glm::vec4 colour = glm::vec4(0.0f); // object colour
	GLfloat roughness = 0.1f;				 // roughness factor
	GLfloat metallic = 0.0f;		         // metallic factor
	GLfloat ao = 1.0f;				         // ambient occlusion factor

	// For materials dictated by textures
	GLuint albedoTex;			// base texture (colour info)
	GLuint detailTex;			// for up close detail texture - currently unused
	GLuint roughnessTex;		// for dictating material roughness value
	GLuint metallicTex;			// for dictating material metallic-ness value
	GLuint emissiveTex;			// for light emission (glow) - currently unused
	GLuint normalMap;			// for surface normals
	GLuint displacementMap;		// for geometry (vertex position) augmentation - currently unused
	GLuint alphaMap;			// for fragment discarding
	GLuint specularMap;			// for specular highlight (lighting) augmentation - currently unused
	GLuint ambientOcclusionMap; // for ambient lighting detail (darkening in crevices) - currently unused


	GLuint skyboxCubeMap;		// for skybox objects only - texture that's mapped onto the skybox
	GLuint skyboxEnvCubeMap;	// for skybox objects only - texture that's used for environment lighting
};
