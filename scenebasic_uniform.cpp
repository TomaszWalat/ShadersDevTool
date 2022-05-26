#include "scenebasic_uniform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "helper/glutils.h"
//#include "helper/texture.h"

#include "imgui/imgui.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


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

	//{"Blinn-Phong Shader",
	//	"shader/blinnPhongShader.vert",
	//	"shader/blinnPhongShader.frag"},


	//{"Blinn-Phong Normal Map Shader",
	//	"shader/blinnPhongShader_normalMap.vert",
	//	"shader/blinnPhongShader_normalMap.frag"},

	{"PBR Shader",
		"shader/pbrShader.vert",
		"shader/pbrShader.frag"},

	{"PBR Texture Shader",
		"shader/pbrTextureShader.vert",
		"shader/pbrTextureShader.frag"},

	{"Skybox Shader",
		"shader/skyboxShader.vert",
		"shader/skyboxShader.frag"},

};



SceneBasic_Uniform::SceneBasic_Uniform() : skybox(100.0f),
										   floor(20.0f, 20.0f, 100, 100),
										   metalCube(2),
										   box(2),
										   torus(0.7f, 0.3f, 25, 25),
										   teapot(10, mat4(1.0f))
{
	piggy = ObjMesh::load("media/pig_triangulated.obj", true);
	ogre = ObjMesh::load("media/bs_ears.obj", false, true);
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
	
	// Setting object textures
	{
		//skybox.material.skyboxCubeMap = Texture::loadHdrCubeMap("../PrototypeShadersDevTool/media/texture/cube/pisa-hdr/pisa");
		//skybox.material.skyboxCubeMap = textures.skybox_MountainLake;
		//skybox.material.skyboxCubeMap = textures.skybox_ArchesE_PineTree;
		skybox.material.skyboxCubeMap = textures.skybox_SummiPool;
		skybox.material.skyboxEnvCubeMap = textures.skybox_env_SummiPool;
		
		floor.material.albedoTex = textures.grayGraniteFlecks_Albedo;
		floor.material.roughnessTex = textures.grayGraniteFlecks_Roughness;
		floor.material.metallicTex = textures.grayGraniteFlecks_Metallic;
		floor.material.normalMap = textures.grayGraniteFlecks_NormalMap;
		////floor.material.displacementMap = textures.grayGraniteFlecks_;
		floor.material.ambientOcclusionMap = textures.grayGraniteFlecks_AmbientOcclusionMap;

		//floor.material.albedoTex = textures.cement;
		//floor.material.detailTex = textures.cement;
		
		metalCube.material.albedoTex = textures.rustedIron_Albedo;
		metalCube.material.roughnessTex = textures.rustedIron_Roughness;
		metalCube.material.metallicTex = textures.rustedIron_Metallic;
		metalCube.material.normalMap = textures.rustedIron_NormalMap;
		////metalCube.material.displacementMap = textures.rustedIron_;
		metalCube.material.ambientOcclusionMap = textures.rustedIron_AmbientOcclusionMap;
		
		//metalCube.material.normalMap = textures.ripple_NormalMap;
		//metalCube.material.displacementMap = textures.ripple_DisplacementMap;
		//metalCube.material.ambientOcclusionMap = textures.ripple_AmbientOcclusionMap;
		//metalCube.material.specularMap = textures.ripple_SpecularMap;
		//metalCube.material.colour = vec4(1.0f);

		box.material.albedoTex = textures.redBrick_Albedo;
		box.material.roughnessTex = textures.redBrick_Roughness;
		box.material.metallicTex = textures.redBrick_Metallic;
		box.material.normalMap = textures.redBrick_NormalMap;
		//box.material.displacementMap = textures.redBrick_HeightMap;
		box.material.ambientOcclusionMap = textures.redBrick_AmbientOcclusionMap;

		//box.material.detailTex = textures.moss;
		//box.material.alphaMap = Texture::loadTexture("../PrototypeShadersDevTool/media/texture/brick/brick1.jpg");
		//box.material.normalMap = textures.brick_NormalMap;
		//box.material.displacementMap = textures.brick_DisplacementMap;
		//box.material.ambientOcclusionMap = textures.brick_AmbientOcclusionMap;
		//box.material.specularMap = textures.brick_SpecularMap;

		torus.material.albedoTex = textures.bambooWood_Albedo;
		torus.material.roughnessTex = textures.bambooWood_Albedo;
		torus.material.metallicTex = textures.bambooWood_Albedo;
		torus.material.normalMap = textures.bambooWood_Albedo;
		//torus.material.displacementMap = textures.bambooWood_Albedo;
		torus.material.ambientOcclusionMap = textures.bambooWood_Albedo;

		//torus.material.detailTex = textures.wood_Albedo;

		teapot.material.albedoTex = textures.copperScuffed_Albedo_Boosted;
		teapot.material.roughnessTex = textures.copperScuffed_Roughness;
		teapot.material.metallicTex = textures.copperScuffed_Metallic;
		teapot.material.normalMap = textures.copperScuffed_NormalMap;
		////teapot.material.displacementMap = textures.copperScuffed_;
		teapot.material.ambientOcclusionMap = textures.copperScuffed_AmbientOcclusionMap;
		teapot.material.alphaMap = textures.alienMetal_AmbientOcclusionMap;

		//teapot.material.detailTex = textures.wood_Albedo;

		piggy->material.albedoTex = textures.scuffedPlastic_Albedo;
		piggy->material.roughnessTex = textures.scuffedPlastic_Roughness;
		piggy->material.metallicTex = textures.scuffedPlastic_Metallic;
		piggy->material.normalMap = textures.scuffedPlastic_NormalMap;
		////piggy->material.displacementMap = textures.scuffedPlastic_;
		piggy->material.ambientOcclusionMap = textures.scuffedPlastic_AmbientOcclusionMap;

		//piggy->material.detailTex = textures.spotCow_Albedo;


		ogre->material.albedoTex = textures.ogre_Albedo;
		ogre->material.roughnessTex = textures.humanSkin_Roughness;
		ogre->material.metallicTex = textures.humanSkin_Metallic;
		ogre->material.normalMap = textures.ogre_NormalMap;
		//ogre->material.displacementMap = textures.humanSkin_HeightMap;
		ogre->material.ambientOcclusionMap = textures.ogre_AmbientOcclusionMap;

		//activate and bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.material.skyboxCubeMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.material.skyboxEnvCubeMap);
	}

	// Positioning objects
	{
		mat4 m = mat4(1.0f);

		skybox.modelMatrix = m;

		m = glm::translate(mat4(1.0f), vec3(0.0f, -1.0f, 0.0f));
		floor.modelMatrix = m;

		m = glm::translate(mat4(1.0f), vec3(-6.0f, 0.0f, -0.5f));
		box.modelMatrix = m;

		m = glm::scale(mat4(1.0f), vec3(2.0f));
		m = glm::translate(m, vec3(-3.0f, 0.95f, 0.0f));
		piggy->modelMatrix = m;

		m = glm::rotate(mat4(1.0f), glm::radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
		m = glm::rotate(m, glm::radians(-45.0f), vec3(0.0f, -1.0f, 1.0f));
		m = glm::translate(m, vec3(-5.0f, 2.5f, -5.0f));
		metalCube.modelMatrix = m;

		m = glm::rotate(mat4(1.0f), glm::radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
		m = glm::translate(m, vec3(0.0f, 2.0f, 0.0f));
		torus.modelMatrix = m;

		m = glm::scale(mat4(1.0f), vec3(2.0f));
		m = glm::rotate(m, glm::radians(-45.0f), vec3(0.0f, 1.0f, 0.0f));
		m = glm::translate(m, vec3(0.0f, 2.0f, -3.0f));
		ogre->modelMatrix = m;

		m = glm::rotate(mat4(1.0f), glm::radians(135.0f), vec3(0.0f, 1.0f, 0.0f));
		m = glm::translate(m, vec3(-6.5f, -1.0f, 3.5f));
		teapot.modelMatrix = m;
	}

	// Setting object material properties
	{
		floor.material.roughness = 1.0f;
		floor.material.metallic = 0.0f;
		
		box.material.roughness = 1.0f;
		box.material.metallic = 0.0f;
		
		piggy->material.roughness = 0.05f;
		piggy->material.metallic = 0.0f;
		piggy->material.colour = vec4(0.95f, 1.0f, 0.5f, 1.0f);
		
		metalCube.material.roughness = 0.05f;
		metalCube.material.metallic = 1.0f;
		metalCube.material.colour = vec4(1.0f);

		torus.material.roughness = 0.5f;
		torus.material.metallic = 0.0f;
		
		ogre->material.roughness = 0.8f;
		ogre->material.metallic = 0.0f;

		teapot.material.roughness = 1.0f;
		teapot.material.metallic = 1.0f;
		teapot.material.colour = vec4(1.0f);
	}

	// Setting up lights
	{
		LightInfo L1 = { // Directional - Sun
			vec4(8.0f, 5.0f, 5.0f, 0.0f),
			vec3(0.0f, 0.0f, 0.0f),
			vec3(1.0f),
			1.0f,
			1.0f, 0.022f, 0.0019f,
			30.0f, 45.0f
		};
		lights.push_back(L1); 

		LightInfo L2 = { // Spot - 1
			vec4(-5.0f, 5.0f, 0.0f, 1.0f),
			vec3(0.0f, -1.0f, 0.0f),
			vec3(1.0f, 0.0f, 0.0f), // Red
			1.0f,
			1.0f, 0.022f, 0.0019f,
			30.0f, 45.0f
		};
		lights.push_back(L2); 

		LightInfo L3 = { // Spot - 2
			vec4(0.0f, 8.0f, -4.5f, 1.0f),
			vec3(0.0f, -1.0f, 0.0f),
			vec3(1.0f),
			1.0f,
			1.0f, 0.022f, 0.0019f,
			30.0f, 45.0f
		};
		lights.push_back(L3);

		LightInfo L4 = { // Spot - 3
			vec4(-6.0f, 6.0f, -6.0f, 1.0f),
			vec3(0.0f, -1.0f, 0.0f),
			vec3(0.0f, 0.0f, 1.0f), // Blue
			1.0f,
			1.0f, 0.022f, 0.0019f,
			30.0f, 45.0f
		};
		lights.push_back(L4);

	}
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

			//progs.at(s).get()->use();
			
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
	// Compute and set object normal space
	mat4 omv = cam->getView() * cam->getModel() * mesh->modelMatrix;
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(omv[0], omv[1], omv[2]));


	progs.at(currentProg)->setUniform("ObjectModelMatrix", mesh->modelMatrix);

	// Set object material properties
	progs.at(currentProg)->setUniform("material.colour", mesh->material.colour);
	progs.at(currentProg)->setUniform("material.roughness", mesh->material.roughness);
	progs.at(currentProg)->setUniform("material.metallic", mesh->material.metallic);


	// Swap out texture bindings

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mesh->material.albedoTex);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, mesh->material.detailTex);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mesh->material.roughnessTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mesh->material.metallicTex);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, mesh->material.normalMap);
	
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, mesh->material.displacementMap);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, mesh->material.ambientOcclusionMap);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, mesh->material.alphaMap);
	

	//mesh = nullptr;
}


void SceneBasic_Uniform::setLights()
{
	std::string uName;
	std::string id = "null";

	progs.at(currentProg)->setUniform("gammaCorrection", gammaCorrection);
	progs.at(currentProg)->setUniform("doHDRToneMapping", doHDRToneMapping);

	for(int i = 0; i < lights.size(); i++)
	{
		id = std::to_string(i);

		uName = "lights[" + id + "].position";
		progs.at(currentProg)->setUniform(uName.c_str(), lights.at(i).position);

		uName = "lights[" + id + "].colour";
		progs.at(currentProg)->setUniform(uName.c_str(), lights.at(i).colour);

		uName = "lights[" + id + "].brightness";
		progs.at(currentProg)->setUniform(uName.c_str(), lights.at(i).brightness);

		uName = "lights[" + id + "].attenuationConstant";
		progs.at(currentProg)->setUniform(uName.c_str(), lights.at(i).attenuationConstant);

		uName = "lights[" + id + "].attenuationLinear";
		progs.at(currentProg)->setUniform(uName.c_str(), lights.at(i).attenuationLinear);

		uName = "lights[" + id + "].attenuationQuadratic";
		progs.at(currentProg)->setUniform(uName.c_str(), lights.at(i).attenuationQuadratic);


		uName = "lights[" + id + "].direction";
		progs.at(currentProg)->setUniform(uName.c_str(), lights.at(i).direction);

		uName = "lights[" + id + "].cutoffInner";
		progs.at(currentProg)->setUniform(uName.c_str(), glm::radians(lights.at(i).cutoffInner));

		uName = "lights[" + id + "].cutoffOuter";
		progs.at(currentProg)->setUniform(uName.c_str(), glm::radians(lights.at(i).cutoffOuter));
	}
}


void SceneBasic_Uniform::drawGUI()
{
	static float alphaDiscard = 0.0f;

	ImGui::Begin("Camera data");
		cam->drawGUI();
	ImGui::End();



	ImGui::Begin("Object Material Info");
	{
		ImGui::SliderFloat("Alpha discard", &alphaDiscard, 0.0f, 1.0f);
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushID(11); // Prevents UI element instance issues
		if(ImGui::CollapsingHeader("Floor"))
		{
			
			ImGui::SliderFloat("Roughness##floor", &floor.material.roughness, 0.05f, 1.0f);
			ImGui::SliderFloat("Metallic##floor", &floor.material.metallic, 0.0f, 1.0f);
			ImGui::ColorEdit4("Colour##floor", glm::value_ptr(floor.material.colour));
		}
		ImGui::PopID(); // Closes instance ID area
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushID(12); // Prevents UI element instance issues
		if(ImGui::CollapsingHeader("Box"))
		{
			ImGui::SliderFloat("Roughness##box", &box.material.roughness, 0.05f, 1.0f);
			ImGui::SliderFloat("Metallic##box", &box.material.metallic, 0.0f, 1.0f);
			ImGui::ColorEdit4("Colour##box", glm::value_ptr(box.material.colour));
		}
		ImGui::PopID(); // Closes instance ID area
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushID(13); // Prevents UI element instance issues
		if(ImGui::CollapsingHeader("Piggy"))
		{
			ImGui::SliderFloat("Roughness##piggy", &piggy->material.roughness, 0.05f, 1.0f);
			ImGui::SliderFloat("Metallic##piggy", &piggy->material.metallic, 0.0f, 1.0f);
			ImGui::ColorEdit4("Colour##piggy", glm::value_ptr(piggy->material.colour));
		}
		ImGui::PopID(); // Closes instance ID area
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushID(14); // Prevents UI element instance issues
		if(ImGui::CollapsingHeader("Metal Cube"))
		{
			ImGui::SliderFloat("Roughness##metalCube", &metalCube.material.roughness, 0.05f, 1.0f);
			ImGui::SliderFloat("Metallic##metalCube", &metalCube.material.metallic, 0.0f, 1.0f);
			ImGui::ColorEdit4("Colour##metalCube", glm::value_ptr(metalCube.material.colour));
		}
		ImGui::PopID(); // Closes instance ID area
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushID(15); // Prevents UI element instance issues
		if(ImGui::CollapsingHeader("Torus"))
		{
			ImGui::SliderFloat("Roughness##torus", &torus.material.roughness, 0.05f, 1.0f);
			ImGui::SliderFloat("Metallic##torus", &torus.material.metallic, 0.0f, 1.0f);
			ImGui::ColorEdit4("Colour##torus", glm::value_ptr(torus.material.colour));
		}
		ImGui::PopID(); // Closes instance ID area
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushID(16); // Prevents UI element instance issues
		if(ImGui::CollapsingHeader("Ogre"))
		{
			ImGui::SliderFloat("Roughness##ogre", &ogre->material.roughness, 0.05f, 1.0f);
			ImGui::SliderFloat("Metallic##ogre", &ogre->material.metallic, 0.0f, 1.0f);
			ImGui::ColorEdit4("Colour##ogre", glm::value_ptr(ogre->material.colour));
		}
		ImGui::PopID(); // Closes instance ID area
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushID(17); // Prevents UI element instance issues
		if(ImGui::CollapsingHeader("Teapot"))
		{
			ImGui::SliderFloat("Roughness##teapot", &teapot.material.roughness, 0.05f, 1.0f);
			ImGui::SliderFloat("Metallic##teapot", &teapot.material.metallic, 0.0f, 1.0f);
			ImGui::ColorEdit4("Colour##teapot", glm::value_ptr(teapot.material.colour));
		}
		ImGui::PopID(); // Closes instance ID area
		ImGui::Separator();
		ImGui::Spacing();
	}
	ImGui::End();



	ImGui::Begin("Light Info");
	{
		ImGui::SliderFloat("Gamma Correction", &gammaCorrection, 0.01f, 5.0f);
		ImGui::Spacing();
		ImGui::Checkbox("HDR Tone Mapping", &doHDRToneMapping);
		ImGui::Separator();
		ImGui::Spacing();

		std::string id = "null";
		for(int i = 0; i < lights.size(); i++)
		{
			ImGui::PushID(i); // Prevents UI element instance issues

			id = std::to_string(i);

			if(ImGui::CollapsingHeader(("Light #" + id).c_str()))
			{
				ImGui::DragFloat4(("Position##L"+ id).c_str(), value_ptr(lights.at(i).position), 0.01f);
				ImGui::DragFloat3(("Direction##L"+ id).c_str(), value_ptr(lights.at(i).direction), 0.01f, -1.0f, 1.0f);
				
				ImGui::Spacing();
				
				ImGui::ColorEdit3(("Colour##L"+ id).c_str(), glm::value_ptr(lights.at(i).colour));
				ImGui::SliderFloat(("Brightness##L"+ id).c_str(), &lights.at(i).brightness, 0.01f, 100.0f);

				ImGui::Spacing();

				ImGui::Text("Attenuation");
				ImGui::SliderFloat(("Constant##L"+ id).c_str(), &lights.at(i).attenuationConstant, 0.0f, 1.0f);
				ImGui::SliderFloat(("Linear##L"+ id).c_str(), &lights.at(i).attenuationLinear, 0.0f, 1.0f);
				ImGui::SliderFloat(("Quadratic##L"+ id).c_str(), &lights.at(i).attenuationQuadratic, 0.0f, 2.0f);
				
				ImGui::Spacing();

				ImGui::Text("Cone cutoff");
				ImGui::SliderFloat(("Inner (Phi)##L"+ id).c_str(), &lights.at(i).cutoffInner, 0.0f, 67.5f);
				ImGui::SliderFloat(("Outer (Gamma)##L"+ id).c_str(), &lights.at(i).cutoffOuter, 0.0f, 67.5f);
			}
			ImGui::PopID();
		}
	}
	ImGui::End();
	


	ImGui::Begin("Shader Selection");
	{
		for(int i = 0; i < shaders.size(); i++)
		{
			if(ImGui::Button(shaders[i][0].c_str()))
			{
				changeShader(shaders[i][0]);
			}
			ImGui::Separator();
			ImGui::Spacing();
		}
	}
	ImGui::End();

	progs.at(currentProg)->setUniform("AlphaDiscard", alphaDiscard);
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
	setLights();


	setMeshUniforms(&floor);
	floor.render();
	
	setMeshUniforms(&box);
	box.render();

	setMeshUniforms(piggy.get());
	piggy->render();

	setMeshUniforms(&torus);
	torus.render();

	setMeshUniforms(&teapot);
	teapot.render();

	
	std::string currentShader = currentProg;
	//changeShader("Blinn-Phong Normal Map Shader");
	//setMatrices();
	//setLights();
	
	setMeshUniforms(&metalCube);
	metalCube.render();

	setMeshUniforms(ogre.get());
	ogre->render();
	
	//changeShader(currentShader);



	//glDepthMask(GL_FALSE);
	changeShader("Skybox Shader");
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", (cam->getProjection() * cam->getView()));
	skybox.render();
	changeShader(currentShader);
	//glDepthMask(GL_TRUE);

}


void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;

    glViewport(0,0,w,h);

	//projection = glm::perspective(glm::radians(70.0f), (float)w/h, 0.3f, 100.0f);
}