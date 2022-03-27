#pragma once

#include <glm/glm.hpp>
#include "materialInfo.h"

class SceneObject
{
public:
	glm::mat4 modelMatrix;
	MaterialInfo material;
};

