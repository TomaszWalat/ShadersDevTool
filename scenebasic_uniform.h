#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include <glad/glad.h>
#include "glm/glm.hpp"

#include "helper/scene.h"
#include "helper/glslprogram.h"

#include "model/torus.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;
    Torus torus;

    void compile();
    void setMatrices();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
