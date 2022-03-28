#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct MaterialInfo
{
	glm::vec4 colour = glm::vec4(0.0f);

	GLfloat ambient = 0.1f;
	GLfloat diffuse = 1.0f;
	GLfloat specular = 1.0f;
	GLfloat shininess = 64.0f;
	GLfloat reflectivity = 0.0f;

	GLuint albedoTex;
	GLuint detailTex;
	GLuint emissiveTex;
	GLuint normalMap;
	GLuint displacementMap;
	GLuint alphaMap;
	GLuint specularMap;
	GLuint ambientOcclusionMap;

	GLuint skyboxCubeMap;
};
