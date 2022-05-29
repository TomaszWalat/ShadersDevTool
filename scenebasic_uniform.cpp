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



// Lists names and paths of all shaders / shader combinations (programs) to be compiled
std::vector<std::vector<std::string>> shaders  {

	{"HDR PBR Textured Shader",
		"shader/hdrpbrTexturedShader.vert",
		"shader/hdrpbrTexturedShader.frag"},
		
	{"HDR PBR (Textureless) Shader",
		"shader/hdrpbrTexturelessShader.vert",
		"shader/hdrpbrTexturelessShader.frag"},

	{"Light Shader",
		"shader/lightShader.vert",
		"shader/lightShader.frag"},

	{"Skybox Shader",
		"shader/skyboxShader.vert",
		"shader/skyboxShader.frag"}

};



SceneBasic_Uniform::SceneBasic_Uniform() : skybox(100.0f),
										   lightSphere(1.0f, 25, 25),
										   floor_1(20.0f, 20.0f, 100, 100),
												metalCube(2), torus(0.7f, 0.3f, 100, 100), teapot(10, mat4(1.0f)), //box(2),
										   floor_2(20.0f, 20.0f, 100, 100),
												ball_1(1.0f, 100, 100),  ball_2(1.0f, 100, 100),  ball_3(1.0f, 100, 100),  ball_4(1.0f, 100, 100),  ball_5(1.0f, 100, 100),
												ball_6(1.0f, 100, 100),  ball_7(1.0f, 100, 100),  ball_8(1.0f, 100, 100),  ball_9(1.0f, 100, 100),  ball_10(1.0f, 100, 100),
												ball_11(1.0f, 100, 100), ball_12(1.0f, 100, 100), ball_13(1.0f, 100, 100), ball_14(1.0f, 100, 100), ball_15(1.0f, 100, 100),
												ball_16(1.0f, 100, 100), ball_17(1.0f, 100, 100), ball_18(1.0f, 100, 100), ball_19(1.0f, 100, 100), ball_20(1.0f, 100, 100), 
												ball_21(1.0f, 100, 100), ball_22(1.0f, 100, 100), ball_23(1.0f, 100, 100), ball_24(1.0f, 100, 100), ball_25(1.0f, 100, 100)
{
	
	cow = ObjMesh::load("media/spot_triangulated.obj", true);
	ogre = ObjMesh::load("media/bs_ears.obj", false, true);
	
}



void SceneBasic_Uniform::initScene()
{
	//// Debugging function
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
	
	// Setting object textures
	{
		//skybox.material.skyboxCubeMap = textures.skybox_MountainLake; // <--- NOT HDR

		//skybox.material.skyboxCubeMap = textures.skybox_PisaHDR;

		skybox.material.skyboxCubeMap = textures.skybox_ArchesE_PineTree;
		skybox.material.skyboxEnvCubeMap = textures.skybox_env_ArchesE_PineTree;

		//skybox.material.skyboxCubeMap = textures.skybox_IceLake;
		//skybox.material.skyboxEnvCubeMap = textures.skybox_env_IceLake;

		//skybox.material.skyboxCubeMap = textures.skybox_Milkyway;
		//skybox.material.skyboxEnvCubeMap = textures.skybox_env_Milkyway;

		//skybox.material.skyboxCubeMap = textures.skybox_MonoLakeC;
		//skybox.material.skyboxEnvCubeMap = textures.skybox_env_MonoLakeC;

		//skybox.material.skyboxCubeMap = textures.skybox_SummiPool;
		//skybox.material.skyboxEnvCubeMap = textures.skybox_env_SummiPool;


		
		floor_1.material.albedoTex = textures.patchyCement_Albedo;
		floor_1.material.roughnessTex = textures.patchyCement_Roughness;
		floor_1.material.metallicTex = textures.patchyCement_Metallic;
		//floor_1.material.normalMap = textures.grayGraniteFlecks_NormalMap;
		////floor_1.material.displacementMap = textures.grayGraniteFlecks_;
		floor_1.material.ambientOcclusionMap = textures.patchyCement_AmbientOcclusionMap;
		{
						
			metalCube.material.albedoTex = textures.aluminumScuffed_Albedo;
			metalCube.material.roughnessTex = textures.aluminumScuffed_Roughness;
			metalCube.material.metallicTex = textures.aluminumScuffed_Metallic;
			//metalCube.material.normalMap = textures.rustedIron_NormalMap;
			////metalCube.material.displacementMap = textures.rustedIron_;
			metalCube.material.ambientOcclusionMap = textures.alienMetal_AmbientOcclusionMap;
			

			//box.material.albedoTex = textures.scuffedPlastic_Albedo;
			//box.material.roughnessTex = textures.scuffedPlastic_Roughness;
			//box.material.metallicTex = textures.scuffedPlastic_Metallic;
			////box.material.normalMap = textures.redBrick_NormalMap;
			////box.material.displacementMap = textures.redBrick_HeightMap;
			//box.material.ambientOcclusionMap = textures.scuffedPlastic_AmbientOcclusionMap;


			torus.material.albedoTex = textures.alienMetal_Albedo;
			torus.material.roughnessTex = textures.alienMetal_Roughness;
			torus.material.metallicTex = textures.alienMetal_Metallic;
			//torus.material.normalMap = textures.mahoganyFloor_NormalMap;
			//torus.material.displacementMap = textures.bambooWood_Albedo;
			torus.material.ambientOcclusionMap = textures.alienMetal_AmbientOcclusionMap;


			teapot.material.albedoTex = textures.copperScuffed_Albedo_Boosted;
			teapot.material.roughnessTex = textures.copperScuffed_Roughness;
			teapot.material.metallicTex = textures.copperScuffed_Metallic;
			//teapot.material.normalMap = textures.copperScuffed_NormalMap;
			////teapot.material.displacementMap = textures.copperScuffed_;
			teapot.material.ambientOcclusionMap = textures.copperScuffed_AmbientOcclusionMap;


			cow->material.albedoTex = textures.cow_Albedo;
			cow->material.roughnessTex = textures.humanSkin_Roughness;
			cow->material.metallicTex = textures.humanSkin_Metallic;
			//cow->material.normalMap = textures.scuffedPlastic_NormalMap;
			////cow->material.displacementMap = textures.scuffedPlastic_;
			cow->material.ambientOcclusionMap = textures.humanSkin_AmbientOcclusionMap;


			ogre->material.albedoTex = textures.ogre_Albedo;
			ogre->material.roughnessTex = textures.humanSkin_Roughness;
			ogre->material.metallicTex = textures.humanSkin_Metallic;
			//ogre->material.normalMap = textures.ogre_NormalMap;
			//ogre->material.displacementMap = textures.humanSkin_HeightMap;
			ogre->material.ambientOcclusionMap = textures.ogre_AmbientOcclusionMap;
		}


		floor_2.material.albedoTex = textures.grayGraniteFlecks_Albedo;
		floor_2.material.roughnessTex = textures.grayGraniteFlecks_Roughness;
		floor_2.material.metallicTex = textures.grayGraniteFlecks_Metallic;
		//floor_2.material.normalMap = textures.patchyCement_NormalMap;
		////floor_2.material.displacementMap = textures.patchyCement_HeightMap;
		floor_2.material.ambientOcclusionMap = textures.grayGraniteFlecks_AmbientOcclusionMap;
		


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

		m = glm::translate(mat4(1.0f), vec3(0.0f, -1.0f, -10.0f));
		floor_1.modelMatrix = m;
		
			//m = glm::translate(mat4(1.0f), vec3(-6.0f, 0.0f, -10.5f));
			//box.modelMatrix = m;

			m = glm::scale(mat4(1.0f), vec3(3.0f));
			m = glm::rotate(m, glm::radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
			m = glm::translate(m, vec3(2.0f, 0.5f, 3.5f));
			cow->modelMatrix = m;

			m = glm::rotate(mat4(1.0f), glm::radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
			m = glm::rotate(m, glm::radians(-45.0f), vec3(0.0f, -1.0f, 1.0f));
			//m = glm::translate(m, vec3(-5.0f, 2.5f, -5.0f));
			m = glm::translate(m, vec3(-5.0f, 10.0f, -12.5f));
			metalCube.modelMatrix = m;

			m = glm::rotate(mat4(1.0f), glm::radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
			m = glm::translate(m, vec3(0.0f, 12.0f, -10.0f));
			torus.modelMatrix = m;

			m = glm::scale(mat4(1.0f), vec3(2.0f));
			m = glm::rotate(m, glm::radians(-45.0f), vec3(0.0f, 1.0f, 0.0f));
			m = glm::translate(m, vec3(-3.5f, 2.0f, -8.0f));
			ogre->modelMatrix = m;

			m = glm::rotate(mat4(1.0f), glm::radians(135.0f), vec3(0.0f, 1.0f, 0.0f));
			m = glm::translate(m, vec3(0.0f, -1.0f, 9.0f));
			teapot.modelMatrix = m;
		


		m = glm::translate(mat4(1.0), vec3(20.0f, -1.0f, 10.0f));
		floor_2.modelMatrix = m;
		
			m = glm::translate(mat4(1.0), vec3(27.0f, 0.0f, 3.0f));
			ball_1.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(27.0f, 0.0f, 6.5f));
			ball_2.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(27.0f, 0.0f, 10.0f));
			ball_3.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(27.0f, 0.0f, 13.5f));
			ball_4.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(27.0f, 0.0f, 17.0f));
			ball_5.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(23.5f, 0.0f, 3.0f));
			ball_6.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(23.5f, 0.0f, 6.5f));
			ball_7.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(23.5f, 0.0f, 10.0f));
			ball_8.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(23.5f, 0.0f, 13.5f));
			ball_9.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(23.5f, 0.0f, 17.0f));
			ball_10.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(20.0f, 0.0f, 3.0f));
			ball_11.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(20.0f, 0.0f, 6.5f));
			ball_12.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(20.0f, 0.0f, 10.0f));
			ball_13.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(20.0f, 0.0f, 13.5f));
			ball_14.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(20.0f, 0.0f, 17.0f));
			ball_15.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(16.5f, 0.0f, 3.0f));
			ball_16.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(16.5f, 0.0f, 6.5f));
			ball_17.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(16.5f, 0.0f, 10.0f));
			ball_18.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(16.5f, 0.0f, 13.5f));
			ball_19.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(16.5f, 0.0f, 17.0f));
			ball_20.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(13.0f, 0.0f, 3.0f));
			ball_21.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(13.0f, 0.0f, 6.5f));
			ball_22.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(13.0f, 0.0f, 10.0f));
			ball_23.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(13.0f, 0.0f, 13.5f));
			ball_24.modelMatrix = m;

			m = glm::translate(mat4(1.0), vec3(13.0f, 0.0f, 17.0f));
			ball_25.modelMatrix = m;
		
		
		//m = glm::translate(mat4(1.0), vec3(-20.0f, -1.0f, 10.0f));
		//floor_3.modelMatrix = m;
	}

	// Setting object material properties
	{
		vec4 ballColour = vec4(1.0f, 0.01f, 0.01f, 1.0f);

		ball_1.material.roughness = 1.0f;
		ball_1.material.metallic = 0.0f;
		ball_1.material.ao = 1.0f;
		ball_1.material.colour = ballColour;

		ball_2.material.roughness = 0.75f;
		ball_2.material.metallic = 0.0f;
		ball_2.material.ao = 1.0f;
		ball_2.material.colour = ballColour;
		
		ball_3.material.roughness = 0.5f;
		ball_3.material.metallic = 0.0f;
		ball_3.material.ao = 1.0f;
		ball_3.material.colour = ballColour;
		
		ball_4.material.roughness = 0.25f;
		ball_4.material.metallic = 0.0f;
		ball_4.material.ao = 1.0f;
		ball_4.material.colour = ballColour;
		
		ball_5.material.roughness = 0.05f;
		ball_5.material.metallic = 0.0f;
		ball_5.material.ao = 1.0f;
		ball_5.material.colour = ballColour;
		
		ball_6.material.roughness = 1.0f;
		ball_6.material.metallic = 0.25f;
		ball_6.material.ao = 1.0f;
		ball_6.material.colour = ballColour;
		
		ball_7.material.roughness = 0.75f;
		ball_7.material.metallic = 0.25f;
		ball_7.material.ao = 1.0f;
		ball_7.material.colour = ballColour;
		
		ball_8.material.roughness = 0.5f;
		ball_8.material.metallic = 0.25f;
		ball_8.material.ao = 1.0f;
		ball_8.material.colour = ballColour;
		
		ball_9.material.roughness = 0.25f;
		ball_9.material.metallic = 0.25f;
		ball_9.material.ao = 1.0f;
		ball_9.material.colour = ballColour;
		
		ball_10.material.roughness = 0.05f;
		ball_10.material.metallic = 0.25f;
		ball_10.material.ao = 1.0f;
		ball_10.material.colour = ballColour;
		
		ball_11.material.roughness = 1.0f;
		ball_11.material.metallic = 0.5f;
		ball_11.material.ao = 1.0f;
		ball_11.material.colour = ballColour;
		
		ball_12.material.roughness = 0.75f;
		ball_12.material.metallic = 0.5f;
		ball_12.material.ao = 1.0f;
		ball_12.material.colour = ballColour;
		
		ball_13.material.roughness = 0.5f;
		ball_13.material.metallic = 0.5f;
		ball_13.material.ao = 1.0f;
		ball_13.material.colour = ballColour;
		
		ball_14.material.roughness = 0.25f;
		ball_14.material.metallic = 0.5f;
		ball_14.material.ao = 1.0f;
		ball_14.material.colour = ballColour;
		
		ball_15.material.roughness = 0.05f;
		ball_15.material.metallic = 0.5f;
		ball_15.material.ao = 1.0f;
		ball_15.material.colour = ballColour;
		
		ball_16.material.roughness = 1.0f;
		ball_16.material.metallic = 0.75f;
		ball_16.material.ao = 1.0f;
		ball_16.material.colour = ballColour;
		
		ball_17.material.roughness = 0.75f;
		ball_17.material.metallic = 0.75f;
		ball_17.material.ao = 1.0f;
		ball_17.material.colour = ballColour;
		
		ball_18.material.roughness = 0.5f;
		ball_18.material.metallic = 0.75f;
		ball_18.material.ao = 1.0f;
		ball_18.material.colour = ballColour;
		
		ball_19.material.roughness = 0.25f;
		ball_19.material.metallic = 0.75f;
		ball_19.material.ao = 1.0f;
		ball_19.material.colour = ballColour;
		
		ball_20.material.roughness = 0.05f;
		ball_20.material.metallic = 0.75f;
		ball_20.material.ao = 1.0f;
		ball_20.material.colour = ballColour;
		
		ball_21.material.roughness = 1.0f;
		ball_21.material.metallic = 1.0f;
		ball_21.material.ao = 1.0f;
		ball_21.material.colour = ballColour;
		
		ball_22.material.roughness = 0.75f;
		ball_22.material.metallic = 1.0f;
		ball_22.material.ao = 1.0f;
		ball_22.material.colour = ballColour;
		
		ball_23.material.roughness = 0.5f;
		ball_23.material.metallic = 1.0f;
		ball_23.material.ao = 1.0f;
		ball_23.material.colour = ballColour;
		
		ball_24.material.roughness = 0.25f;
		ball_24.material.metallic = 1.0f;
		ball_24.material.ao = 1.0f;
		ball_24.material.colour = ballColour;
		
		ball_25.material.roughness = 0.05f;
		ball_25.material.metallic = 1.0f;
		ball_25.material.ao = 1.0f;
		ball_25.material.colour = ballColour;
	}

	// Setting up lights
	{
		LightInfo L1 = { // Directional - Sun
			vec4(5.0f, 20.0f, 5.0f, 0.0f),
			vec3(0.0f, 0.0f, 0.0f),
			vec3(1.0f),
			5.0f,
			1.0f, 0.022f, 0.0019f,
			30.0f, 45.0f
		};
		lights.push_back(L1); 


		LightInfo L2 = { // Spot - 1 (floor 1)
			vec4(3.5f, 9.0f, -15.0f, 1.0f),
			vec3(0.0f, -1.0f, 0.0f),
			vec3(1.0f, 0.0f, 0.0f), // Red
			2.0f,
			1.0f, 0.022f, 0.0019f,
			41.0f, 47.0f
		};
		lights.push_back(L2); 

		LightInfo L3 = { // Spot - 2 (floor 1)
			vec4(-2.5f, 13.0f, -10.0f, 1.0f),
			vec3(0.0f, -1.0f, 0.0f),
			vec3(1.0f), // White
			1.5f,
			1.0f, 0.022f, 0.0019f,
			48.0f, 51.0f
		};
		lights.push_back(L3);

		LightInfo L4 = { // Spot - 3 (floor 1)
			vec4(5.0f, 11.0f, -8.0f, 1.0f),
			vec3(-0.5f, -1.0f, -0.5f),
			vec3(0.0f, 0.0f, 1.0f), // Blue
			2.5f,
			1.0f, 0.022f, 0.0019f,
			48.0f, 53.0f
		};
		lights.push_back(L4);


		LightInfo L5 = { // Point - 1 (floor 2)
			vec4(20.0f, 6.0f, 30.0f, 1.0f),
			vec3(0.0f, 0.0f, 0.0f),
			vec3(1.0f), // White
			1.0f,
			1.0f, 0.022f, 0.0019f,
			30.0f, 45.0f
		};
		lights.push_back(L5);
		
		LightInfo L6 = { // Point - 2 (floor 2)
			vec4(40.0f, 2.0f, 6.0f, 1.0f),
			vec3(0.0f, 0.0f, 0.0f),
			vec3(1.0f), // White
			1.0f,
			1.0f, 0.022f, 0.0019f,
			30.0f, 45.0f
		};
		lights.push_back(L6);
		
		//LightInfo L7 = { // Point - 3 (floor 2)
		//	vec4(-6.0f, 6.0f, -6.0f, 1.0f),
		//	vec3(0.0f, -1.0f, 0.0f),
		//	vec3(0.0f, 0.0f, 1.0f), // Blue
		//	1.0f,
		//	1.0f, 0.022f, 0.0019f,
		//	30.0f, 45.0f
		//};
		//lights.push_back(L4);
		
		//LightInfo L8 = { // Point - 4 (floor 2)
		//	vec4(-6.0f, 6.0f, -6.0f, 1.0f),
		//	vec3(0.0f, -1.0f, 0.0f),
		//	vec3(0.0f, 0.0f, 1.0f), // Blue
		//	1.0f,
		//	1.0f, 0.022f, 0.0019f,
		//	30.0f, 45.0f
		//};
		//lights.push_back(L4);
		
		//LightInfo L9 = { // Point - 5 (floor 3)
		//	vec4(-6.0f, 6.0f, -6.0f, 1.0f),
		//	vec3(0.0f, -1.0f, 0.0f),
		//	vec3(0.0f, 0.0f, 1.0f), // Blue
		//	1.0f,
		//	1.0f, 0.022f, 0.0019f,
		//	30.0f, 45.0f
		//};
		//lights.push_back(L4);
	}


	setupFBO();
	setupFrameQuad();
	setupSamplers();
	computeWeights();
	setWeights();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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




// Sets up HDR / bloom FBO
void SceneBasic_Uniform::setupFBO()
{
	// --- HDR FBO setup --- //

	// Create and bind FBO (HDR)
	glGenFramebuffers(1, &bufferTextures.hdr_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);

	// Depth buffer
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// HDR colour texture
	glActiveTexture(GL_TEXTURE8);
	glGenTextures(1, &bufferTextures.hdr_Colour);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.hdr_Colour);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
	// Blur texture one
	glActiveTexture(GL_TEXTURE9);
	glGenTextures(1, &bufferTextures.blur_One);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.blur_One);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);

	// Blur texture two
	glActiveTexture(GL_TEXTURE10);
	glGenTextures(1, &bufferTextures.blur_Two);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.blur_Two);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);

	// Attach depth, HDR and blur textures to frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferTextures.hdr_Colour, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferTextures.blur_One, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, bufferTextures.blur_Two, 0);

	// Specify which buffers (textures) are being drawn to
	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(4, drawBuffers);
	
	// Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


// Sets up a quad for post-processing
void SceneBasic_Uniform::setupFrameQuad()
{
	// Vertex and TexCoord arrays for full-screen quad
	GLfloat vert[] = {
		-1.0f, -1.0f, 0.0f,		// bottom-left
		1.0f, -1.0f, 0.0f,		// bottom-right
		1.0f, 1.0f, 0.0f,			// top-right

		-1.0f, -1.0f, 0.0f,		// bottom-left
		1.0f, 1.0f, 0.0f,		// top-right
		-1.0f, 1.0f, 0.0f		// top-left
	};
	GLfloat tc[] = {
		0.0f, 0.0f,		// bottom-left
		1.0f, 0.0f,		// bottom-right
		1.0f, 1.0f,		// top-right

		0.0f, 0.0f,		// bottom-left
		1.0f, 1.0f,		// top-right
		0.0f, 1.0f		// top-left
	};

	// Set up the buffers
	unsigned int handle[2];
	glGenBuffers(2, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), vert, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

	// Set up the vertex array object
	glGenVertexArrays(1, &bufferTextures.frame_Quad);
	glBindVertexArray(bufferTextures.frame_Quad);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0); // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2); // Texture coordinates

	// Revert to default framebuffer
	glBindVertexArray(0);
}


// Sets up two sampler objects for linear and nearest filtering
void SceneBasic_Uniform::setupSamplers()
{
	GLuint samplers[2];
	glGenSamplers(2, samplers);

	bufferTextures.linear_Sampler = samplers[0];
	bufferTextures.nearest_Sampler = samplers[1];

	GLfloat border[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Set up the nearest sampler
	glSamplerParameteri(bufferTextures.nearest_Sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(bufferTextures.nearest_Sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(bufferTextures.nearest_Sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glSamplerParameteri(bufferTextures.nearest_Sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glSamplerParameterfv(bufferTextures.nearest_Sampler, GL_TEXTURE_BORDER_COLOR, border);

	// Set up the linear sampler
	glSamplerParameteri(bufferTextures.linear_Sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(bufferTextures.linear_Sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(bufferTextures.linear_Sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glSamplerParameteri(bufferTextures.linear_Sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glSamplerParameterfv(bufferTextures.linear_Sampler, GL_TEXTURE_BORDER_COLOR, border);

	//// We want nearest sampling except for the last pass.
	//glBindSampler(0, bufferTextures.linear_Sampler);
	//glBindSampler(1, bufferTextures.nearest_Sampler);
	//glBindSampler(2, bufferTextures.nearest_Sampler);
	//glBindSampler(3, bufferTextures.nearest_Sampler);
	//glBindSampler(4, bufferTextures.nearest_Sampler);
	//glBindSampler(5, bufferTextures.nearest_Sampler);
	//glBindSampler(6, bufferTextures.nearest_Sampler);
	//glBindSampler(7, bufferTextures.nearest_Sampler);
	//glBindSampler(8, bufferTextures.nearest_Sampler);
	//glBindSampler(9, bufferTextures.nearest_Sampler);
	//glBindSampler(10, bufferTextures.nearest_Sampler);

}


// Computes and sums the blur weights
void SceneBasic_Uniform::computeWeights()
{
	weights[0] = gauss(0, sigma2);

	sum = weights[0];

	for (int i = 1; i < 10; i++) 
	{
		weights[i] = gauss(i, sigma2);
		sum += 2 * weights[i];
	}
}


// Gaussian blur weight function
float SceneBasic_Uniform::gauss(float x, float sigma2)
{
	double coeff = 1.0 / (glm::two_pi<double>() * sigma2);

	double expon = -(x * x) / (2.0 * sigma2);

	return (float)(coeff * exp(expon));
}


// Computes (HDR) texture log of average lumen
void SceneBasic_Uniform::computeLogAvgLuminance()
{
	int size = width * height;

	std::vector<GLfloat> texData(size * 4);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.hdr_Colour);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, texData.data());

	float sum = 0.0f;
	for (int i = 0; i < size; i++) 
	{
		float lum = glm::dot(vec3(texData[i * 4 + 0], texData[i * 4 + 1],	texData[i * 4 + 2]), vec3(0.2126f, 0.7152f, 0.0722f));
		sum += logf(lum + 0.00001f);
	}

	progs.at(currentProg)->setUniform("AverageLumen", expf(sum / size));
}




// Sets "global" uniforms (mainly matrices, mainly camera related)
void SceneBasic_Uniform::setMatrices()
{
	mat4 mv = cam->getView() * cam->getModel();

	progs.at(currentProg)->setUniform("ModelViewMatrix", mv);
	progs.at(currentProg)->setUniform("ViewMatrix", cam->getView());
	progs.at(currentProg)->setUniform("ModelMatrix", cam->getModel());
	progs.at(currentProg)->setUniform("MVP", cam->getProjection() * mv);
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", (cam->getProjection() * cam->getView()));
	progs.at(currentProg)->setUniform("SkyboxRotationMatrix", (cam->getView() * skyboxRotate180Y));
}


// Sets per object uniforms and textures
void SceneBasic_Uniform::setMeshUniforms(TriangleMesh* mesh)
{

	// Compute and set object normal space
	mat4 omv = cam->getView() * cam->getModel() * mesh->modelMatrix;
	progs.at(currentProg)->setUniform("NormalMatrix", mat3(omv[0], omv[1], omv[2]));

	// Position and location of model within the world space (i.e. within the cam's Model matrix)
	progs.at(currentProg)->setUniform("ObjectModelMatrix", mesh->modelMatrix);

	// Uniforms only used if textured based PBR is not used
	if(!usePBRTextures)
	{
		// Set object material properties
		progs.at(currentProg)->setUniform("material.albedo", mesh->material.colour);
		progs.at(currentProg)->setUniform("material.roughness", mesh->material.roughness);
		progs.at(currentProg)->setUniform("material.metallic", mesh->material.metallic);
		progs.at(currentProg)->setUniform("material.ao", mesh->material.ao);
		
	}

	// Textures only used if textured based PBR is being used
	if(usePBRTextures)
	{
		// Set textures
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh->material.albedoTex);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mesh->material.roughnessTex);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, mesh->material.metallicTex);

		//glActiveTexture(GL_TEXTURE4);
		//glBindTexture(GL_TEXTURE_2D, mesh->material.normalMap);
		
		//glActiveTexture(GL_TEXTURE5);
		//glBindTexture(GL_TEXTURE_2D, mesh->material.displacementMap);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, mesh->material.ambientOcclusionMap);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, mesh->material.alphaMap);
	}

}


// Sets scene light uniforms
void SceneBasic_Uniform::setLights()
{
	std::string uName;
	std::string id = "null";

	progs.at(currentProg)->setUniform("GammaCorrection", gammaCorrection);
	
	progs.at(currentProg)->setUniform("DoHDRToneMapping", doHDRToneMapping);
	progs.at(currentProg)->setUniform("Exposure", hdrExposure);

	progs.at(currentProg)->setUniform("DoBloom", doBloom);
	progs.at(currentProg)->setUniform("LuminanceThreshold", luminanceThreshold);
	progs.at(currentProg)->setUniform("BloomOneStrength", bloomBlurOneStrength);
	progs.at(currentProg)->setUniform("BloomTwoStrength", bloomBlurTwoStrength);

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


// Sets the blur weights - should only need to set once
// (maybe also when shaders are changed)
void SceneBasic_Uniform::setWeights()
{
	std::string uName;
	std::string index = "";

	// Normalize the weights
	for (int i = 0; i < 10; i++) 
	{
		index = std::to_string(i);

		uName = "Weight[" + index + "]";

		float value = weights[i] / sum;

		progs.at(currentProg)->setUniform(uName.c_str(), value);
	}
}




// Draw scene to HDR texture, draw bright pixels to blur one texture
void SceneBasic_Uniform::drawPassOne()
{
	progs.at(currentProg)->setUniform("PassNo", 1);

	// Set weights in case shader context has been changed (since calculation)
	setWeights();

	// Bind HDR frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);

	// Set viewport size
	glViewport(0, 0, width, height);

	// Prepare for rendering to HDR frame buffer

	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Specify which buffers (textures) are being drawn to
	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_NONE };
	glDrawBuffers(4, drawBuffers);
	
	// Draw scene to HDR framebuffer
	drawScene();
		
	// Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


// Blur pass one - read from blur texture one, blur it, write it to blur texture two
void SceneBasic_Uniform::drawPassTwo()
{
	progs.at(currentProg)->setUniform("PassNo", 2);

	// Bind HDR frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);
	
	glDisable(GL_DEPTH_TEST); // Disable depth testing as otherwise the blur textures aren't displayed

	// Remove all camera based movement / rotation / projection for drawing to frame quad
	mat4 m = mat4(1.0f);
	progs.at(currentProg)->setUniform("NormalMatrix", m);
	progs.at(currentProg)->setUniform("ObjectModelMatrix", m);
	progs.at(currentProg)->setUniform("ModelMatrix", m);
	progs.at(currentProg)->setUniform("ModelViewMatrix", m);
	progs.at(currentProg)->setUniform("ViewMatrix", m);
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", m);
	progs.at(currentProg)->setUniform("MVP", m);
	
	// Bind blur one texture for reading from
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.blur_One);
	
	// Specify which buffers (textures) are being drawn to
	GLenum drawBuffers[] = { GL_NONE, GL_NONE, GL_NONE, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(4, drawBuffers);
	
	// Read from blur texture one, using linear sampling for extra blur
	glBindSampler(9, bufferTextures.linear_Sampler);

	// Render to the full screen quad
	glBindVertexArray(bufferTextures.frame_Quad);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Revert to nearest sampling
	glBindSampler(9, bufferTextures.nearest_Sampler);

	// Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


// Blur pass two - read from blur texture two, blur it, write it to blur texture one
void SceneBasic_Uniform::drawPassThree()
{
	progs.at(currentProg)->setUniform("PassNo", 3);

	// Bind HDR frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);

	// Bind blur two texture for reading from
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.blur_Two);
	
	// Specify which buffers (textures) are being drawn to
	GLenum drawBuffers[] = { GL_NONE, GL_NONE, GL_COLOR_ATTACHMENT1, GL_NONE };
	glDrawBuffers(4, drawBuffers);
		
	// Read from blur texture one, using linear sampling for extra blur
	glBindSampler(10, bufferTextures.linear_Sampler);

	// Render to the full screen quad
	glBindVertexArray(bufferTextures.frame_Quad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	// Revert to nearest sampling
	glBindSampler(10, bufferTextures.nearest_Sampler);
	
	// Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


// Read from blur texture one and HDR texture, tone map the HDR and mix with blur
void SceneBasic_Uniform::drawPassFour()
{
	progs.at(currentProg)->setUniform("PassNo", 4);
	
	// Bind default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Bind HDR and blur one texture for reading from
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.hdr_Colour);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.blur_One);

	// Read from blur texture one, using linear sampling for extra blur
	glBindSampler(9, bufferTextures.linear_Sampler);
	
	// Render to the full screen quad
	glBindVertexArray(bufferTextures.frame_Quad);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Revert to nearest sampling
	glBindSampler(9, bufferTextures.nearest_Sampler);
}


// Draw scene objects
void SceneBasic_Uniform::drawScene()
{
	std::string currentShader = currentProg;

	setMatrices();
	setLights();
	
	setMeshUniforms(&floor_1);
	floor_1.render();
	
	//setMeshUniforms(&box);
	//box.render();

	setMeshUniforms(cow.get());
	cow->render();

	setMeshUniforms(&torus);
	torus.render();

	setMeshUniforms(&teapot);
	teapot.render();
	
	setMeshUniforms(&metalCube);
	metalCube.render();

	setMeshUniforms(ogre.get());
	ogre->render();


	setMeshUniforms(&floor_2);
	floor_2.render();


	changeShader("HDR PBR (Textureless) Shader");
	usePBRTextures = false;
	
	setMatrices();
	setLights();

	setMeshUniforms(&ball_1);
	ball_1.render();
	setMeshUniforms(&ball_2);
	ball_2.render();
	setMeshUniforms(&ball_3);
	ball_3.render();
	setMeshUniforms(&ball_4);
	ball_4.render();
	setMeshUniforms(&ball_5);
	ball_5.render();
	setMeshUniforms(&ball_6);
	ball_6.render();
	setMeshUniforms(&ball_7);
	ball_7.render();
	setMeshUniforms(&ball_8);
	ball_8.render();
	setMeshUniforms(&ball_9);
	ball_9.render();
	setMeshUniforms(&ball_10);
	ball_10.render();
	setMeshUniforms(&ball_11);
	ball_11.render();
	setMeshUniforms(&ball_12);
	ball_12.render();
	setMeshUniforms(&ball_13);
	ball_13.render();
	setMeshUniforms(&ball_14);
	ball_14.render();
	setMeshUniforms(&ball_15);
	ball_15.render();
	setMeshUniforms(&ball_16);
	ball_16.render();
	setMeshUniforms(&ball_17);
	ball_17.render();
	setMeshUniforms(&ball_18);
	ball_18.render();
	setMeshUniforms(&ball_19);
	ball_19.render();
	setMeshUniforms(&ball_20);
	ball_20.render();
	setMeshUniforms(&ball_21);
	ball_21.render();
	setMeshUniforms(&ball_22);
	ball_22.render();
	setMeshUniforms(&ball_23);
	ball_23.render();
	setMeshUniforms(&ball_24);
	ball_24.render();
	setMeshUniforms(&ball_25);
	ball_25.render();

	usePBRTextures = true;


	changeShader("Light Shader");
	setMatrices();
	mat4 m = mat4(1.0);
	progs.at(currentProg)->setUniform("LuminanceThreshold", luminanceThreshold);

	for(int i = 0; i < lights.size(); i++)
	{
		m[3] = lights[i].position;
		m[3][3] = 1.0f;

		progs.at(currentProg)->setUniform("ObjectModelMatrix", m);
		progs.at(currentProg)->setUniform("LightType", lights[i].position.w);
		progs.at(currentProg)->setUniform("LightColour", lights[i].colour);
		progs.at(currentProg)->setUniform("Brightness", lights[i].brightness);
		progs.at(currentProg)->setUniform("Direction", lights[i].direction);
		progs.at(currentProg)->setUniform("CutoffInner", glm::radians(lights[i].cutoffInner));
		progs.at(currentProg)->setUniform("CutoffOuter", glm::radians(lights[i].cutoffOuter));

		lightSphere.render();
	}



	changeShader("Skybox Shader");
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", (cam->getProjection() * cam->getView() * skyboxRotate180Y));
	skybox.render();
	changeShader(currentShader);
}


// Draw GUI panels (Dear ImGui does all the heavy lifting)
void SceneBasic_Uniform::drawGUI()
{
	static float alphaDiscard = 0.0f;

	ImGui::Begin("Camera data");
		cam->drawGUI();
	ImGui::End();


	ImGui::Begin("Object Material Info");
	{
		//ImGui::SliderFloat("Alpha discard", &alphaDiscard, 0.0f, 1.0f);
		//ImGui::Separator();
		//ImGui::Spacing();

		ImGui::PushID(11); // Prevents UI element instance issues
		if(ImGui::CollapsingHeader("Floor"))
		{
			
			ImGui::SliderFloat("Roughness##floor1", &floor_1.material.roughness, 0.05f, 1.0f);
			ImGui::SliderFloat("Metallic##floor1", &floor_1.material.metallic, 0.0f, 1.0f);
			ImGui::ColorEdit4("Colour##floor1", glm::value_ptr(floor_1.material.colour));
		}
		ImGui::PopID(); // Closes instance ID area
		ImGui::Separator();
		ImGui::Spacing();

		//ImGui::PushID(12); // Prevents UI element instance issues
		//if(ImGui::CollapsingHeader("Box"))
		//{
		//	ImGui::SliderFloat("Roughness##box", &box.material.roughness, 0.05f, 1.0f);
		//	ImGui::SliderFloat("Metallic##box", &box.material.metallic, 0.0f, 1.0f);
		//	ImGui::ColorEdit4("Colour##box", glm::value_ptr(box.material.colour));
		//}
		//ImGui::PopID(); // Closes instance ID area
		//ImGui::Separator();
		//ImGui::Spacing();

		ImGui::PushID(13); // Prevents UI element instance issues
		if(ImGui::CollapsingHeader("Piggy"))
		{
			ImGui::SliderFloat("Roughness##cow", &cow->material.roughness, 0.05f, 1.0f);
			ImGui::SliderFloat("Metallic##cow", &cow->material.metallic, 0.0f, 1.0f);
			ImGui::ColorEdit4("Colour##cow", glm::value_ptr(cow->material.colour));
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
		ImGui::Spacing();
		ImGui::Checkbox("Bloom effect", &doBloom);
		ImGui::Spacing();
		ImGui::SliderFloat("HDR Exposure", &hdrExposure, 0.01f, 2.0f);
		ImGui::Separator();
		ImGui::SliderFloat("Bloom Threshold", &luminanceThreshold, 0.01f, 5.0f);
		ImGui::Separator();
		ImGui::SliderInt("Bloom axis level", &bloomBlurOneStrength, 1, 10);
		ImGui::Separator();
		ImGui::SliderInt("Bloom diagonal level", &bloomBlurTwoStrength, 1, 10);
		ImGui::Separator();
		ImGui::Separator();

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
				ImGui::SliderFloat(("Brightness##L"+ id).c_str(), &lights.at(i).brightness, 0.01f, 15.0f);

				ImGui::Spacing();

				ImGui::Text("Attenuation");
				ImGui::SliderFloat(("Constant##L"+ id).c_str(), &lights.at(i).attenuationConstant, 0.0f, 1.0f);
				ImGui::SliderFloat(("Linear##L"+ id).c_str(), &lights.at(i).attenuationLinear, 0.0f, 1.0f);
				ImGui::SliderFloat(("Quadratic##L"+ id).c_str(), &lights.at(i).attenuationQuadratic, 0.0f, 2.0f);
				
				ImGui::Spacing();

				ImGui::Text("Cone cutoff");
				ImGui::SliderFloat(("Inner (Phi)##L"+ id).c_str(), &lights.at(i).cutoffInner, 0.0f, 67.4f);
				ImGui::SliderFloat(("Outer (Gamma)##L"+ id).c_str(), &lights.at(i).cutoffOuter, 0.1f, 67.5f);
			}
			ImGui::PopID();
		}
	}
	ImGui::End();
	


	//ImGui::Begin("Shader Selection");
	//{
	//	for(int i = 0; i < shaders.size(); i++)
	//	{
	//		if(ImGui::Button(shaders[i][0].c_str()))
	//		{
	//			changeShader(shaders[i][0]);
	//		}
	//		ImGui::Separator();
	//		ImGui::Spacing();
	//	}
	//}
	//ImGui::End();

	//progs.at(currentProg)->setUniform("AlphaDiscard", alphaDiscard);
}


// Executes various draw functions
void SceneBasic_Uniform::render()
{

	drawGUI();

	drawPassOne();
	
	computeLogAvgLuminance();
	
	drawPassTwo();

	drawPassThree();

	drawPassFour();
}




// Update game loop - for physics and/or animations
void SceneBasic_Uniform::update( float t )
{
	//update your angle here
}


// Resize viewport
void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;

    glViewport(0,0,w,h);

	//projection = glm::perspective(glm::radians(70.0f), (float)w/h, 0.3f, 100.0f);
}


// Clean up memory
// (or at least that's the idea, but the actual texture data is not deleted by glDelete___ ).
// so there is still a memory leak :(
SceneBasic_Uniform::~SceneBasic_Uniform()
{
	glDeleteFramebuffers(1, &bufferTextures.hdr_FBO);
	glDeleteVertexArrays(1, &bufferTextures.frame_Quad);
	glDeleteSamplers(1, &bufferTextures.linear_Sampler);
	glDeleteSamplers(1, &bufferTextures.nearest_Sampler);

	glDeleteTextures(1, &bufferTextures.hdr_Colour);
	glDeleteTextures(1, &bufferTextures.blur_One);
	glDeleteTextures(1, &bufferTextures.blur_Two);

	glDeleteTextures(1, &textures.skybox_MountainLake);
	glDeleteTextures(1, &textures.skybox_PisaHDR);
	//glDeleteTextures(1, &textures.brick_Albedo);
	glDeleteTextures(1, &textures.ogre_Albedo);
	glDeleteTextures(1, &textures.ogre_NormalMap);
	glDeleteTextures(1, &textures.ogre_AmbientOcclusionMap);
	//glDeleteTextures(1, &textures.ripple_NormalMap);
	//glDeleteTextures(1, &textures.wood_Albedo);
	//glDeleteTextures(1, &textures.fire);
	//glDeleteTextures(1, &textures.star);
	//glDeleteTextures(1, &textures.cement);

	glDeleteTextures(1, &textures.alienMetal_Albedo);
	glDeleteTextures(1, &textures.alienMetal_Roughness);
	glDeleteTextures(1, &textures.alienMetal_Metallic);
	//glDeleteTextures(1, &textures.alienMetal_NormalMap);
	//glDeleteTextures(1, &textures.alienMetal_HeightMap);
	glDeleteTextures(1, &textures.alienMetal_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.copperScuffed_Albedo);
	glDeleteTextures(1, &textures.copperScuffed_Albedo_Boosted);
	glDeleteTextures(1, &textures.copperScuffed_Roughness);
	glDeleteTextures(1, &textures.copperScuffed_Metallic);
	//glDeleteTextures(1, &textures.copperScuffed_NormalMap);
	glDeleteTextures(1, &textures.copperScuffed_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.rustedIron_Albedo);
	glDeleteTextures(1, &textures.rustedIron_Roughness);
	glDeleteTextures(1, &textures.rustedIron_Metallic);
	//glDeleteTextures(1, &textures.rustedIron_NormalMap);
	glDeleteTextures(1, &textures.rustedIron_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.bambooWood_Albedo);
	glDeleteTextures(1, &textures.bambooWood_Roughness);
	glDeleteTextures(1, &textures.bambooWood_Metallic);
	//glDeleteTextures(1, &textures.bambooWood_NormalMap);
	glDeleteTextures(1, &textures.bambooWood_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.redBrick_Albedo);
	glDeleteTextures(1, &textures.redBrick_Roughness);
	glDeleteTextures(1, &textures.redBrick_Metallic);
	//glDeleteTextures(1, &textures.redBrick_NormalMap);
	//glDeleteTextures(1, &textures.redBrick_HeightMap);
	glDeleteTextures(1, &textures.redBrick_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.grayGraniteFlecks_Albedo);
	glDeleteTextures(1, &textures.grayGraniteFlecks_Roughness);
	glDeleteTextures(1, &textures.grayGraniteFlecks_Metallic);
	//glDeleteTextures(1, &textures.grayGraniteFlecks_NormalMap);
	glDeleteTextures(1, &textures.grayGraniteFlecks_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.scuffedPlastic_Albedo);
	glDeleteTextures(1, &textures.scuffedPlastic_Roughness);
	glDeleteTextures(1, &textures.scuffedPlastic_Metallic);
	//glDeleteTextures(1, &textures.scuffedPlastic_NormalMap);
	glDeleteTextures(1, &textures.scuffedPlastic_AmbientOcclusionMap);

	//glDeleteTextures(1, &textures.humanSkin_Albedo);
	glDeleteTextures(1, &textures.humanSkin_Roughness);
	glDeleteTextures(1, &textures.humanSkin_Metallic);
	//glDeleteTextures(1, &textures.humanSkin_NormalMap);
	//glDeleteTextures(1, &textures.humanSkin_HeightMap);
	glDeleteTextures(1, &textures.humanSkin_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.skybox_ArchesE_PineTree);
	glDeleteTextures(1, &textures.skybox_env_ArchesE_PineTree);
	glDeleteTextures(1, &textures.skybox_SummiPool);
	glDeleteTextures(1, &textures.skybox_env_SummiPool);

}