#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct LightInfo
{
	// location in world; z = 0 for directional light (overrides other types)
	glm::vec4 position = glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);

	// direction it's pointing; xyz = 0 for point light (overrides spotlight)
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);		

	glm::vec3 colour = glm::vec3(1.0f); // colour of light


	// light intensity drop off (attenuation) properties (point and spot light only)
	// https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
	GLfloat attenuationConstant = 1.0f;
	GLfloat attenuationLinear = 0.022f;
	GLfloat attenuationQuadratic = 0.0019f;

	// light cone properties (spotlight only). phi < gamma
	GLfloat cutoffInner = 30.0f; // Phi
	GLfloat cutoffOuter = 45.0f; // Gamma
};