#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "helper/glutils.h"

#include "imgui/imgui.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 30, 30) {}

struct MetrialInfo
{
	vec3 Ka = vec3(0.1f, 0.1f, 1.0f);
	vec3 Kd = vec3(0.1f, 0.1f, 1.0f);
	vec3 Ks = vec3(1.0f, 1.0f, 1.0f);
} torusMaterial;

struct LightInfo
{
	vec3 La = vec3(0.1f, 0.1f, 0.1f);
	vec3 Ld = vec3(0.5f, 0.5f, 0.5f);
	vec3 Ls = vec3(1.0f, 1.0f, 1.0f);
	vec4 Position = vec4(5.0f, 5.0f, 5.0f, 1.0f);
} light;

void SceneBasic_Uniform::initScene()
{
    compile();

	glEnable(GL_DEPTH_TEST);

	model = mat4(1.0f);
	//model[3][1] = -2.5f;

	view = lookAt(vec3(0.0f, 0.0f, 2.50f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	projection = mat4(1.0f);

	prog.setUniform("Ka", vec3(0.1f, 0.1f, 1.0f));
	prog.setUniform("Kd", vec3(0.1f, 0.1f, 1.0f));
	prog.setUniform("Ks", vec3(1.0f, 1.0f, 1.0f));

	prog.setUniform("La", vec3(0.1f, 0.1f, 0.1f));
	prog.setUniform("Ld", vec3(0.5f, 0.5f, 0.5f));
	prog.setUniform("Ls", vec3(1.0f, 1.0f, 1.0f));
	prog.setUniform("LightPosition", model * view * vec4(5.0f, 5.0f, 2.0f, 1.0f));
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basicShader.vert");
		prog.compileShader("shader/basicShader.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::setMatrices()
{
	mat4 mv = view * model;

	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::drawGUI()
{
	ImGui::Begin("Torus Info");

	ImGui::ColorEdit3("Ambient", glm::value_ptr(torusMaterial.Ka));
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(torusMaterial.Kd));
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::ColorEdit3("Specular", glm::value_ptr(torusMaterial.Ks));

	ImGui::End();

	prog.setUniform("Ka", torusMaterial.Ka);
	prog.setUniform("Kd", torusMaterial.Kd);
	prog.setUniform("Ks", torusMaterial.Ks);

	ImGui::Begin("Light Info");
	
	ImGui::ColorEdit3("Ambient", glm::value_ptr(light.La));
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.Ld));
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::ColorEdit3("Specular", glm::value_ptr(light.Ls));
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::DragFloat4("Position", glm::value_ptr(light.Position), 0.01f);

	ImGui::End();

	prog.setUniform("La", light.La);
	prog.setUniform("Ld", light.Ld);
	prog.setUniform("Ls", light.Ls);
	prog.setUniform("LightPosition", model * view * light.Position);
}


void SceneBasic_Uniform::update( float t )
{
	//update your angle here
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGUI();

	setMatrices();

	torus.render();

}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;

    glViewport(0,0,w,h);

	projection = glm::perspective(glm::radians(70.0f), (float)w/h, 0.3f, 100.0f);
}
