#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct LightInfo
{
	glm::vec4 position;
	glm::vec3 direction;

	glm::vec3 colour;
	GLfloat ambient;
	GLfloat diffuse;
	GLfloat specular;

	GLfloat attenuationConstant;
	GLfloat attenuationLinear;
	GLfloat attenuationQuadratic;
};