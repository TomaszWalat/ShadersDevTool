#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include <memory>
#include <glad/glad.h>
#include "glm/glm.hpp"

#include "helper/scene.h"
#include "helper/glslprogram.h"

#include "model/torus.h"

class SceneBasic_Uniform : public Scene
{
private:
    //GLuint currentProg = 0;
    std::string currentProg;
    GLSLProgram prog;
    std::map<std::string, std::unique_ptr<GLSLProgram>> progs;
    Torus torus;

    void compile();
    void setMatrices();
    void drawGUI();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void changeShader(std::string shaderName);
};

#endif // SCENEBASIC_UNIFORM_H
