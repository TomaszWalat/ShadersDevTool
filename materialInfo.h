#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct MaterialInfo
{
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	GLfloat shinniness;
};
