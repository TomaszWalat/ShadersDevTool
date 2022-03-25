#include "scenebasic_uniform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "helper/glutils.h"

#include "imgui/imgui.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


struct MetrialInfo
{
	vec3 Ka = vec3(0.1f, 0.1f, 1.0f);
	vec3 Kd = vec3(0.1f, 0.1f, 1.0f);
	vec3 Ks = vec3(1.0f, 1.0f, 1.0f);
	GLfloat Shininess = 64.0f;
} torusMaterial;

struct LightInfo
{
	vec3 La = vec3(0.1f, 0.1f, 0.1f);
	vec3 Ld = vec3(0.5f, 0.5f, 0.5f);
	vec3 Ls = vec3(1.0f, 1.0f, 1.0f);
	vec4 Position = vec4(5.0f, 5.0f, 5.0f, 0.0f);
} light;

std::vector<std::vector<std::string>> shaders  {
	//{"Basic Shader",
	//	"shader/basicShader.vert",
	//	"shader/basicShader.frag"},

	//{"Basic Flat Shader",
	//	"shader/basicFlatShader.vert",
	//	"shader/basicFlatShader.frag"},

	//{"Gouraud Shader",
	//	"shader/gouraudShader.vert",
	//	"shader/gouraudShader.frag"},

	//{"Gouraud Flat Shader",
	//	"shader/gouraudFlatShader.vert",
	//	"shader/gouraudFlatShader.frag"},

	//{"Phong Shader",
	//	"shader/phongShader.vert",
	//	"shader/phongShader.frag"},

	//{"Phong Flat Shader",					// <---- KIND OF POINLESS
	//	"shader/phongFlatShader.vert",
	//	"shader/phongFlatShader.frag"},

	{"Blinn-Phong Shader",
		"shader/blinnPhongShader.vert",
		"shader/blinnPhongShader.frag"},

	{"Blinn-Phong Flat Shader",
		"shader/blinnPhongFlatShader.vert",
		"shader/blinnPhongFlatShader.frag"},
};

SceneBasic_Uniform::SceneBasic_Uniform() : floor(20.0f, 20.0f, 100, 100),
										   torus(0.7f, 0.3f, 25, 25),
										   teapot(10, mat4(1.0f))
{
	piggy = ObjMesh::load("../PrototypeShadersDevTool/media/pig_triangulated.obj", true);

	mat4 m = mat4(1.0f);
	
	m = glm::rotate(m, glm::radians(135.0f), vec3(0.0f, 1.0f, 0.0f));
	m = glm::translate(m, vec3(-5.0f, 0.0f, 3.0f));

	teapot.model = m;
	
	torus.model = glm::translate(mat4(1.0f), vec3(0.0f, 2.0f, -2.0f));
	
	piggy->model = glm::translate(glm::scale(mat4(1.0f), vec3(2.0f)), vec3(-2.0f, 0.5f, 0.0f));
}

void SceneBasic_Uniform::initScene()
{

	//std::cout << shaders.size() << " by " << shaders[0].size() << std::endl;
	//for(int i = 0; i < shaders.size(); i++)
	//{
	//	std::cout << i << " = { ";
	//	for(int j = 0; j < shaders[i].size(); j++)
	//	{
	//		std::cout << shaders[i][j] << ", ";
	//	}
	//	std::cout << " };" << std::endl;
	//}

	cam = std::make_unique<Camera>();

    compile();

	glEnable(GL_DEPTH_TEST);

	//model = mat4(1.0f);
	//model[3][1] = -2.5f;

	//view = lookAt(vec3(0.0f, 0.0f, 2.50f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	//projection = mat4(1.0f);
	
	progs.at(currentProg)->setUniform("Ka", vec3(0.1f, 0.1f, 1.0f));
	progs.at(currentProg)->setUniform("Kd", vec3(0.1f, 0.1f, 1.0f));
	progs.at(currentProg)->setUniform("Ks", vec3(1.0f, 1.0f, 1.0f));
	progs.at(currentProg)->setUniform("Shininess", 64.0f);

	progs.at(currentProg)->setUniform("La", vec3(0.1f, 0.1f, 0.1f));
	progs.at(currentProg)->setUniform("Ld", vec3(0.5f, 0.5f, 0.5f));
	progs.at(currentProg)->setUniform("Ls", vec3(1.0f, 1.0f, 1.0f));
	progs.at(currentProg)->setUniform("LightPosition", vec4(5.0f, 5.0f, 2.0f, 0.0f));

	/*progs[currentProg].setUniform("Ka", vec3(0.1f, 0.1f, 1.0f));
	progs[currentProg].setUniform("Kd", vec3(0.1f, 0.1f, 1.0f));
	progs[currentProg].setUniform("Ks", vec3(1.0f, 1.0f, 1.0f));

	progs[currentProg].setUniform("La", vec3(0.1f, 0.1f, 0.1f));
	progs[currentProg].setUniform("Ld", vec3(0.5f, 0.5f, 0.5f));
	progs[currentProg].setUniform("Ls", vec3(1.0f, 1.0f, 1.0f));
	progs[currentProg].setUniform("LightPosition", model * view * vec4(5.0f, 5.0f, 2.0f, 1.0f));*/
}

void SceneBasic_Uniform::compile()
{
	for(int i = 0; i < shaders.size(); i++)
	{
		try {
			std::unique_ptr<GLSLProgram> p = std::make_unique<GLSLProgram>();
			
			std::string s = shaders[i][0];

			progs.insert(std::map<std::string, std::unique_ptr<GLSLProgram>>::value_type(s, std::move(p)));

			progs.at(s)->compileShader(shaders[i][1].c_str());
			progs.at(s)->compileShader(shaders[i][2].c_str());
			progs.at(s)->link();
			//progs.at(s)->use();

			//progs[currentProg].compileShader("shader/basicShader.vert");
			//progs[currentProg].compileShader("shader/basicShader.frag");
			//progs[currentProg].link();
			//progs[currentProg].use();
		} catch (GLSLProgramException &e) {
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	

	currentProg = shaders[0][0];
	progs.at(currentProg)->use();
}

void SceneBasic_Uniform::changeShader(std::string shaderName)
{
	//std::map<std::string, std::unique_ptr<GLSLProgram>>::iterator i = progs.find(shaderName);
	auto iterator = progs.find(shaderName);

	if(iterator != progs.end())
	{
		std::cout << "found: " << shaderName << std::endl;
		currentProg = iterator->first;
		iterator->second->use();
	}
}


void SceneBasic_Uniform::setMatrices()
{
	mat4 mv = cam->getView() * cam->getModel();

	progs.at(currentProg)->setUniform("ModelViewMatrix", mv);
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	progs.at(currentProg)->setUniform("MVP", cam->getProjection() * mv);

	/*progs[currentProg].setUniform("ModelViewMatrix", mv);
	progs[currentProg].setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	progs[currentProg].setUniform("MVP", projection * mv);*/
}

void SceneBasic_Uniform::drawGUI()
{
	ImGui::Begin("Camera data");
	cam->drawGUI();
	ImGui::End();

	ImGui::Begin("Torus Info");

	ImGui::ColorEdit3("Ambient", glm::value_ptr(torusMaterial.Ka));
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(torusMaterial.Kd));
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::ColorEdit3("Specular", glm::value_ptr(torusMaterial.Ks));
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::SliderFloat("Shininess", &torusMaterial.Shininess, 0.0f, 256.0f);

	ImGui::End();

	progs.at(currentProg)->setUniform("Ka", torusMaterial.Ka);
	progs.at(currentProg)->setUniform("Kd", torusMaterial.Kd);
	progs.at(currentProg)->setUniform("Ks", torusMaterial.Ks);
	progs.at(currentProg)->setUniform("Shininess", torusMaterial.Shininess);

	//progs[currentProg].setUniform("Ka", torusMaterial.Ka);
	//progs[currentProg].setUniform("Kd", torusMaterial.Kd);
	//progs[currentProg].setUniform("Ks", torusMaterial.Ks);

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

	progs.at(currentProg)->setUniform("La", light.La);
	progs.at(currentProg)->setUniform("Ld", light.Ld);
	progs.at(currentProg)->setUniform("Ls", light.Ls);
	progs.at(currentProg)->setUniform("LightPosition", light.Position);

	/*progs[currentProg].setUniform("La", light.La);
	progs[currentProg].setUniform("Ld", light.Ld);
	progs[currentProg].setUniform("Ls", light.Ls);
	progs[currentProg].setUniform("LightPosition", model * view * light.Position);*/

	ImGui::Begin("Shader Selection");

	for(int i = 0; i < shaders.size(); i++)
	{
		if(ImGui::Button(shaders[i][0].c_str()))
		{
			changeShader(shaders[i][0]);
		}
		ImGui::Separator();
		ImGui::Spacing();
	}

	ImGui::End();

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
	
	progs.at(currentProg)->setUniform("ObjectModelMatrix", floor.model);
	mat4 mv = cam->getView() * cam->getModel() * floor.model;
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	floor.render();

	progs.at(currentProg)->setUniform("ObjectModelMatrix", torus.model);
	mv = cam->getView() * cam->getModel() * torus.model;
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	torus.render();

	progs.at(currentProg)->setUniform("ObjectModelMatrix", teapot.model);
	mv = cam->getView() * cam->getModel() * teapot.model;
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	teapot.render();
	
	progs.at(currentProg)->setUniform("ObjectModelMatrix", piggy->model);
	mv = cam->getView() * cam->getModel() * piggy->model;
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	piggy->render();

}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;

    glViewport(0,0,w,h);

	projection = glm::perspective(glm::radians(70.0f), (float)w/h, 0.3f, 100.0f);
}
