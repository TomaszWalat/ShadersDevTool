#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct MaterialInfo
{
	glm::vec4 colour = glm::vec4(0.0f); // object base colour, set alpha to 0 to discard

	GLfloat ambient = 0.1f;		 // ambient light reflectivity factor
	GLfloat diffuse = 1.0f;		 // diffuse light reflectivity factor
	GLfloat specular = 1.0f;	 // specular light reflectivity factor
	GLfloat shininess = 64.0f;	 // specular highlight reflectivity factor
	GLfloat reflectivity = 0.0f; // ignore - unused for now, meant for skybox reflectivity

	GLuint albedoTex;			// base texture (colour info)
	GLuint detailTex;			// for up close detail texture - currently unused
	GLuint emissiveTex;			// for light emission (glow) - currently unused
	GLuint normalMap;			// for surface normals
	GLuint displacementMap;		// for geometry (vertex position) augmentation - currently unused
	GLuint alphaMap;			// for fragment discarding
	GLuint specularMap;			// for specular highlight (lighting) augmentation - currently unused
	GLuint ambientOcclusionMap; // for ambient lighting detail (darkening in crevices) - currently unused

	GLuint skyboxCubeMap;		// for skybox objects only - texture that's mapped onto the skybox
};
