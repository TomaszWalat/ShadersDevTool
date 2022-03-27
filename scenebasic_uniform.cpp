#include "scenebasic_uniform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "helper/glutils.h"
#include "helper/texture.h"

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

	//{"Blinn-Phong Flat Shader",
	//	"shader/blinnPhongFlatShader.vert",
	//	"shader/blinnPhongFlatShader.frag"},

	{"Skybox Blinn-Phong Shader",
		"shader/blinnPhongSkyboxReflectionShader.vert",
		"shader/blinnPhongSkyboxReflectionShader.frag"},

	{"Skybox Shader",
		"shader/skyboxShader.vert",
		"shader/skyboxShader.frag"},

	{"(experimental) Blinn-Phong Shader",
		"shader/blinnPhongShader.vert",
		"shader/blinnPhongShader.frag"},

};

SceneBasic_Uniform::SceneBasic_Uniform() : skybox(100.0f),
										   floor(20.0f, 20.0f, 100, 100),
										   metalCube(2),
										   box(2),
										   torus(0.7f, 0.3f, 25, 25),
										   teapot(10, mat4(1.0f))
{
	piggy = ObjMesh::load("../PrototypeShadersDevTool/media/pig_triangulated.obj", true);
	ogre = ObjMesh::load("../PrototypeShadersDevTool/media/bs_ears.obj", false, true);
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

	skybox.material.skyboxCubeMap = Texture::loadHdrCubeMap("../PrototypeShadersDevTool/media/texture/cube/pisa-hdr/pisa");
	//skybox.material.skyboxCubeMap = Texture::loadCubeMap("../PrototypeShadersDevTool/media/texture/cube/pisa/pisa");
	
	floor.material.albedoTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/cement.jpg");
	floor.material.detailTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/cement.jpg");
	//floor.material.detailTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/cube/pisa/pisa_negx.png");

	metalCube.material.normalMap = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/rippleCube/NormalMap.png");
	metalCube.material.displacementMap = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/rippleCube/DisplacementMap.png");
	metalCube.material.ambientOcclusionMap = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/rippleCube/AmbientOcclusionMap.png");
	metalCube.material.specularMap = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/rippleCube/SpecularMap.png");

	box.material.albedoTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/brick1.jpg");
	box.material.detailTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/moss.png");
	box.material.alphaMap = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/fire.png");

	torus.material.albedoTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/hardwood2_diffuse.jpg");
	torus.material.detailTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/hardwood2_roughness.jpg");

	teapot.material.albedoTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/hardwood2_diffuse.jpg");
	teapot.material.detailTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/hardwood2_diffuse.jpg");
	teapot.material.alphaMap = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/bluewater.png");

	piggy->material.albedoTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/hardwood2_diffuse.jpg");
	piggy->material.detailTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/hardwood2_diffuse.jpg");


	ogre->material.albedoTex = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/ogre/ogre_diffuse.png");
	ogre->material.normalMap = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/ogre/ogre_normalmap.png");

	//activate and bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.material.skyboxCubeMap);

	mat4 m = mat4(1.0f);

	skybox.modelMatrix = m;

	m = glm::translate(mat4(1.0f), vec3(0.0f, -5.0f, 0.0f));
	floor.modelMatrix = m;

	m = glm::rotate(mat4(1.0f), glm::radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
	m = glm::rotate(m, glm::radians(-45.0f), vec3(0.0f, -1.0f, 1.0f));
	m = glm::translate(m, vec3(-5.0f, 2.5f, -5.0f));
	box.modelMatrix = m;

	m = glm::rotate(mat4(1.0f), glm::radians(135.0f), vec3(0.0f, 1.0f, 0.0f));
	m = glm::translate(m, vec3(-5.0f, 0.0f, 3.0f));
	teapot.modelMatrix = m;
	
	m = glm::rotate(mat4(1.0f), glm::radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
	m = glm::translate(m, vec3(0.0f, 2.0f, 0.0f));
	torus.modelMatrix = m;

	m = glm::scale(mat4(1.0f), vec3(2.0f));
	m = glm::translate(m, vec3(-2.0f, 0.5f, 0.0f));
	piggy->modelMatrix = m;

	m = glm::scale(mat4(1.0f), vec3(2.0f));
	//m = glm::translate(m, vec3(-2.0f, 0.5f, 0.0f));
	ogre->modelMatrix = m;

	//floor.material.ambient = 0.1f;
	//floor.material.diffuse = 1.0f;
	//floor.material.specular = 1.0f;
	//floor.material.shininess = 64.0f;
	//
	//box.material.ambient = 0.1f;
	//box.material.diffuse = 1.0f;
	//box.material.specular = 1.0f;
	//box.material.shininess = 64.0f;
	//
	//torus.material.ambient = 0.1f;
	//torus.material.diffuse = 1.0f;
	//torus.material.specular = 1.0f;
	//torus.material.shininess = 64.0f;
	//
	//teapot.material.ambient = 0.1f;
	//teapot.material.diffuse = 1.0f;
	//teapot.material.specular = 1.0f;
	//teapot.material.shininess = 64.0f;
	//
	//piggy->material.ambient = 0.1f;
	//piggy->material.diffuse = 1.0f;
	//piggy->material.specular = 1.0f;
	//piggy->material.shininess = 64.0f;
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
			progs.at(s)->use();

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
		//std::cout << "found: " << shaderName << std::endl;
		currentProg = iterator->first;
		iterator->second->use();
	}
}


void SceneBasic_Uniform::setMatrices()
{
	mat4 mv = cam->getView() * cam->getModel();

	progs.at(currentProg)->setUniform("ModelViewMatrix", mv);
	progs.at(currentProg)->setUniform("ViewMatrix", cam->getView());
	progs.at(currentProg)->setUniform("ModelMatrix", cam->getModel());
	progs.at(currentProg)->setUniform("MVP", cam->getProjection() * mv);
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", (cam->getProjection() * cam->getView()));
	//progs.at(currentProg)->setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	//progs.at(currentProg)->setUniform("ModelMatrix", cam->getModel());
	//progs.at(currentProg)->setUniform("ViewMatrix", cam->getView());
	//progs.at(currentProg)->setUniform("ProjectionMatrix", cam->getProjection());
	/*progs[currentProg].setUniform("ModelViewMatrix", mv);
	progs[currentProg].setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	progs[currentProg].setUniform("MVP", projection * mv);*/
}

void SceneBasic_Uniform::setMeshUniforms(TriangleMesh* mesh)
{
	mat4 omv = cam->getView() * cam->getModel() * mesh->modelMatrix;

	progs.at(currentProg)->setUniform("NormalMatrix", mat3(omv[0], omv[1], omv[2]));
	progs.at(currentProg)->setUniform("ObjectModelMatrix", mesh->modelMatrix);
	
	progs.at(currentProg)->setUniform("KAmbient", mesh->material.ambient);
	progs.at(currentProg)->setUniform("KDiffuse", mesh->material.diffuse);
	progs.at(currentProg)->setUniform("KSpecular", mesh->material.specular);
	progs.at(currentProg)->setUniform("KShininess", mesh->material.shininess);
	progs.at(currentProg)->setUniform("KColour", mesh->material.colour);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mesh->material.albedoTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mesh->material.detailTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mesh->material.alphaMap);
	
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, mesh->material.normalMap);



	mesh = nullptr;
}

void SceneBasic_Uniform::drawGUI()
{
	static float alphaDiscard = 0.0f;

	ImGui::Begin("Camera data");
	cam->drawGUI();
	ImGui::End();

	ImGui::Begin("Object Material Info");

	ImGui::SliderFloat("Alpha discard", &alphaDiscard, 0.0f, 1.0f);

	ImGui::Separator();
	ImGui::Spacing();

	progs.at(currentProg)->setUniform("AlphaDiscard", alphaDiscard);

	//ImGui::ColorEdit3("Ambient", glm::value_ptr(torusMaterial.Ka));
	//ImGui::Separator();
	//ImGui::Spacing();
	//ImGui::ColorEdit3("Diffuse", glm::value_ptr(torusMaterial.Kd));
	//ImGui::Separator();
	//ImGui::Spacing();
	//ImGui::ColorEdit3("Specular", glm::value_ptr(torusMaterial.Ks));
	//ImGui::Separator();
	//ImGui::Spacing();
	//ImGui::SliderFloat("Shininess", &torusMaterial.Shininess, 0.0f, 256.0f);


	if(ImGui::CollapsingHeader("Floor"))
	{
		ImGui::SliderFloat("Ambient##floor", &floor.material.ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse##floor", &floor.material.diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular##floor", &floor.material.specular, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess##floor", &floor.material.shininess, 0.0f, 256.0f);
		ImGui::ColorEdit3("Colour##floor", glm::value_ptr(floor.material.colour));
	}
	ImGui::Separator();
	ImGui::Spacing();

	if(ImGui::CollapsingHeader("Metal Cube"))
	{
		ImGui::SliderFloat("Ambient##metalCube", &metalCube.material.ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse##metalCube", &metalCube.material.diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular##metalCube", &metalCube.material.specular, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess##metalCube", &metalCube.material.shininess, 0.0f, 256.0f);
		ImGui::ColorEdit3("Colour##metalCube", glm::value_ptr(metalCube.material.colour));
	}
	ImGui::Separator();
	ImGui::Spacing();

	if(ImGui::CollapsingHeader("Box"))
	{
		ImGui::SliderFloat("Ambient##box", &box.material.ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse##box", &box.material.diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular##box", &box.material.specular, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess##box", &box.material.shininess, 0.0f, 256.0f);
		ImGui::ColorEdit3("Colour##box", glm::value_ptr(box.material.colour));
	}
	ImGui::Separator();
	ImGui::Spacing();

	if(ImGui::CollapsingHeader("Torus"))
	{
		ImGui::SliderFloat("Ambient##torus", &torus.material.ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse##torus", &torus.material.diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular##torus", &torus.material.specular, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess##torus", &torus.material.shininess, 0.0f, 256.0f);
		ImGui::ColorEdit3("Colour##torus", glm::value_ptr(torus.material.colour));
	}
	ImGui::Separator();
	ImGui::Spacing();

	if(ImGui::CollapsingHeader("Teapot"))
	{
		ImGui::SliderFloat("Ambient##teapot", &teapot.material.ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse##teapot", &teapot.material.diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular##teapot", &teapot.material.specular, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess##teapot", &teapot.material.shininess, 0.0f, 256.0f);
		ImGui::ColorEdit3("Colour##teapot", glm::value_ptr(teapot.material.colour));
	}
	ImGui::Separator();
	ImGui::Spacing();

	if(ImGui::CollapsingHeader("Piggy"))
	{
		ImGui::SliderFloat("Ambient##piggy", &piggy->material.ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse##piggy", &piggy->material.diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular##piggy", &piggy->material.specular, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess##piggy", &piggy->material.shininess, 0.0f, 256.0f);
		ImGui::ColorEdit3("Colour##piggy", glm::value_ptr(piggy->material.colour));
	}
	ImGui::Separator();
	ImGui::Spacing();
	
	if(ImGui::CollapsingHeader("Ogre"))
	{
		ImGui::SliderFloat("Ambient##ogre", &ogre->material.ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse##ogre", &ogre->material.diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular##ogre", &ogre->material.specular, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess##ogre", &ogre->material.shininess, 0.0f, 256.0f);
		ImGui::ColorEdit3("Colour##ogre", glm::value_ptr(ogre->material.colour));
	}
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::End();


	//progs.at(currentProg)->setUniform("Ka", torusMaterial.Ka);
	//progs.at(currentProg)->setUniform("Kd", torusMaterial.Kd);
	//progs.at(currentProg)->setUniform("Ks", torusMaterial.Ks);
	//progs.at(currentProg)->setUniform("Shininess", torusMaterial.Shininess);

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

	/*
	progs.at(currentProg)->setUniform("ObjectModelMatrix", floor.modelMatrix);
	mat4 mv = cam->getView() * cam->getModel() * floor.modelMatrix;
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	floor.render();

	progs.at(currentProg)->setUniform("ObjectModelMatrix", torus.modelMatrix);
	mv = cam->getView() * cam->getModel() * torus.modelMatrix;
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	torus.render();

	progs.at(currentProg)->setUniform("ObjectModelMatrix", teapot.modelMatrix);
	mv = cam->getView() * cam->getModel() * teapot.modelMatrix;
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	teapot.render();
	
	progs.at(currentProg)->setUniform("ObjectModelMatrix", piggy->modelMatrix);
	mv = cam->getView() * cam->getModel() * piggy->modelMatrix;
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(mv[0], mv[1], mv[2]));
	piggy->render();*/

	//glDepthMask(GL_FALSE);
	std::string currentShader = currentProg;
	changeShader("Skybox Shader");
	//progs.at("Skybox Shader")->use();
	//setMatrices();
	//progs.at(currentProg)->setUniform("MVP", cam->getProjection() * cam->getView() * cam->getModel());
	//progs.at(currentProg)->setUniform("ModelMatrix", cam->getModel());
	//progs.at(currentProg)->setUniform("ViewMatrix", cam->getView());
	//progs.at(currentProg)->setUniform("ProjectionMatrix", cam->getProjection());
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", (cam->getProjection() * cam->getView()));
	//setMeshUniforms(&skybox);
	////activate and bindtexture
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.material.skyboxCubeMap);*/
	skybox.render();
	changeShader(currentShader);
	//progs.at(currentShader)->use();
	//glDepthMask(GL_TRUE);

	setMatrices();

	setMeshUniforms(&floor);
	floor.render();

	//setMeshUniforms(&metalCube);
	//metalCube.render();

	setMeshUniforms(&box);
	box.render();

	setMeshUniforms(&torus);
	torus.render();

	setMeshUniforms(&teapot);
	teapot.render();

	setMeshUniforms(piggy.get());
	piggy->render();

	changeShader("(experimental) Blinn-Phong Shader");
	setMatrices();
	progs.at(currentProg)->setUniform("La", light.La);
	progs.at(currentProg)->setUniform("Ld", light.Ld);
	progs.at(currentProg)->setUniform("Ls", light.Ls);
	progs.at(currentProg)->setUniform("LightPosition", light.Position);
	setMeshUniforms(ogre.get());
	ogre->render();
	changeShader(currentShader);

}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;

    glViewport(0,0,w,h);

	projection = glm::perspective(glm::radians(70.0f), (float)w/h, 0.3f, 100.0f);
}

