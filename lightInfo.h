#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct LightInfo
{
	glm::vec4 position = glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

	glm::vec3 colour = glm::vec3(1.0f);
	GLfloat ambient = 0.1f;
	GLfloat diffuse = 0.75f;
	GLfloat specular = 1.0f;


	// https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
	GLfloat attenuationConstant = 1.0f;
	GLfloat attenuationLinear = 0.022f;
	GLfloat attenuationQuadratic = 0.0019f;

	GLfloat cutoffInner = 30.0f; // Phi
	GLfloat cutoffOuter = 45.0f; // Gamma
};