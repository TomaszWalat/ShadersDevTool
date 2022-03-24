#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct LightInfo
{
	glm::vec3 colour;
	GLfloat La;
	GLfloat Ld;
	GLfloat Ls;
};