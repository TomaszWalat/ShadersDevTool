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


	{"PBR + HDR Shader",
		"shader/hdrShader.vert",
		"shader/hdrShader.frag"},

	{"PBR Texture Shader",
		"shader/pbrTextureShader.vert",
		"shader/pbrTextureShader.frag"},

	{"PBR Shader",
		"shader/pbrShader.vert",
		"shader/pbrShader.frag"},

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

	//glClearColor(0.3, 0.75, 0.68, 1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	
	// Setting object textures
	{
		////skybox.material.skyboxCubeMap = Texture::loadHdrCubeMap("../PrototypeShadersDevTool/media/texture/cube/pisa-hdr/pisa");
		////skybox.material.skyboxCubeMap = textures.skybox_MountainLake;
		//skybox.material.skyboxCubeMap = textures.skybox_ArchesE_PineTree;
		//skybox.material.skyboxEnvCubeMap = textures.skybox_env_ArchesE_PineTree;
		//skybox.material.skyboxCubeMap = textures.skybox_SummiPool;
		skybox.material.skyboxEnvCubeMap = textures.skybox_env_SummiPool;
		skybox.material.skyboxCubeMap = textures.skybox_PisaHDR;
		//skybox.material.skyboxEnvCubeMap = textures.skybox_PisaHDR;
		
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
			10.0f,
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

	setupFBOs();
	setupFrameQuad();
	setupSamplers();
	computeWeights();
	setWeights();
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

void SceneBasic_Uniform::setupFBOs()
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



	//GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0 };
	////GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	//glDrawBuffers(2, drawBuffers);

	// --- Blur FBO setup --- //

	//// Create and bind FBO (Bright-pass)
	//glGenFramebuffers(1, &bufferTextures.blur_FBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, bufferTextures.blur_FBO);

	// Calculate size of blur textures
	bloomBufferWidth = width / 4;
	bloomBufferHeight = height / 4;

	// Blur texture one
	glActiveTexture(GL_TEXTURE9);
	glGenTextures(1, &bufferTextures.blur_One);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.blur_One);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
	//glGenerateTextureMipmap(GL_TEXTURE9);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, bloomBufferWidth, bloomBufferHeight);

	// Blur texture two
	glActiveTexture(GL_TEXTURE10);
	glGenTextures(1, &bufferTextures.blur_Two);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.blur_Two);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
	//glGenerateTextureMipmap(GL_TEXTURE10);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, bloomBufferWidth, bloomBufferHeight);

	// Attach depth & hdr images to frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferTextures.hdr_Colour, 0);
	//// Bind blur texture one to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferTextures.blur_One, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, bufferTextures.blur_Two, 0);
	
	//GLenum drawBuffers2[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	//GLenum drawBuffers2[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	//GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	GLenum drawBuffers2[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(4, drawBuffers2);
	//glDrawBuffers(4, drawBuffers);

	/*if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		glClearColor(0.75f, 0.3f, 0.68f, 1.0f);
	}*/

	// Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void SceneBasic_Uniform::setupFrameQuad()
{
	// Vertex and TexCoord arrays for full-screen quad
	GLfloat vert[] = {
		-1.0f, -1.0f, 0.0f,		// bottom-left
		1.0f, -1.0f, 0.0f,		// bottom-right
		1.0f, 1.0f, 0.0f,		// top-right

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

	glBindVertexArray(0); // Set back to default 

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

// Computes and sums the weights
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

// Gaussian blur function
float SceneBasic_Uniform::gauss(float x, float sigma2)
{
	double coeff = 1.0 / (glm::two_pi<double>() * sigma2);

	double expon = -(x * x) / (2.0 * sigma2);

	return (float)(coeff * exp(expon));
}


void SceneBasic_Uniform::computeLogAvgLuminance()
{
	int size = width * height;

	std::vector<GLfloat> texData(size * 3);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.hdr_Colour);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, texData.data());

	float sum = 0.0f;
	for (int i = 0; i < size; i++) 
	{
		float lum = glm::dot(vec3(texData[i * 3 + 0], texData[i * 3 + 1],	texData[i * 3 + 2]), vec3(0.2126f, 0.7152f, 0.0722f));
		sum += logf(lum + 0.00001f);
	}

	progs.at(currentProg)->setUniform("AverageLumen", expf(sum / size));
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
	progs.at(currentProg)->setUniform("SkyboxRotationMatrix", (cam->getView() * skyboxRotate180Y));
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



	//// Swap out texture bindings
	//glBindFramebuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);

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
	progs.at(currentProg)->setUniform("doBloom", doBloom);
	progs.at(currentProg)->setUniform("skyboxBrightness", skyboxBrightness);
	progs.at(currentProg)->setUniform("Exposure", hdrExposure);
	progs.at(currentProg)->setUniform("LuminanceThreshold", luminanceThreshold);
	progs.at(currentProg)->setUniform("bloomAxisBlur", bloomAxisBlur);
	progs.at(currentProg)->setUniform("bloomDiagonalBlur", bloomDiagonalBlur);
	//progs.at(currentProg)->setUniform("bloomMipmapLevel", bloomMipmapLevel);

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


// Draw scene to HDR texture
void SceneBasic_Uniform::drawPassOne()
{
	progs.at(currentProg)->setUniform("PassNo", 1);
	setWeights();
	// Bind HDR frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);

	// Set viewport size
	glViewport(0, 0, width, height);

	// Prepare for rendering to HDR frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	/*
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.hdr_Colour);*/


	// Set framebuffer colour data to hdr colour texture
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferTextures.hdr_Colour, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferTextures.blur_One, 0);

	GLenum drawBuffers2[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_NONE };
	glDrawBuffers(4, drawBuffers2);

	//view = glm::lookAt(vec3(2.0f, 0.0f, 14.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.3f, 100.0f);

	//// Draw scene to HDR frame buffer
	drawScene();
	
	//glClear(GL_COLOR_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	
	// Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

// Extract bright data from HDR texture, copy it to blur texture one
void SceneBasic_Uniform::drawPassTwo()
{
	progs.at(currentProg)->setUniform("PassNo", 2);
	
	// Bind HDR frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);
	glDisable(GL_DEPTH_TEST);

	//glBindBuffer(GL_FRAMEBUFFER, bufferTextures.blur_FBO);
	//glBindBuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);


	////// Prepare for rendering
	//glViewport(0, 0, bloomBufferWidth, bloomBufferHeight);
	// Prepare for rendering to HDR frame buffer
	//glClear(GL_COLOR_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT);

	// Set framebuffer colour data to blur one texture
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferTextures.blur_One, 0);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.hdr_Colour);

	GLenum drawBuffers2[] = { GL_NONE, GL_NONE, GL_COLOR_ATTACHMENT1, GL_NONE };
	glDrawBuffers(4, drawBuffers2);

	

	
	mat4 m = mat4(1.0f);
	progs.at(currentProg)->setUniform("NormalMatrix", m);
	progs.at(currentProg)->setUniform("ObjectModelMatrix", m);
	progs.at(currentProg)->setUniform("ModelMatrix", m);
	progs.at(currentProg)->setUniform("ModelViewMatrix", m);
	progs.at(currentProg)->setUniform("ViewMatrix", m);
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", m);
	progs.at(currentProg)->setUniform("MVP", m);
	setWeights();


	// Read from blur texture one, using linear sampling for extra blur
	glBindSampler(8, bufferTextures.linear_Sampler);


	// Render to the full screen quad
	glBindVertexArray(bufferTextures.frame_Quad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	// Revert to nearest sampling
	glBindSampler(8, bufferTextures.nearest_Sampler);


	//// Revert to default framebuffer
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);

	//mat4 m = mat4(1.0f);
	//progs.at(currentProg)->setUniform("NormalMatrix", m);
	//progs.at(currentProg)->setUniform("ObjectModelMatrix", m);
	//progs.at(currentProg)->setUniform("ModelMatrix", m);
	//progs.at(currentProg)->setUniform("ModelViewMatrix", m);
	//progs.at(currentProg)->setUniform("ViewMatrix", m);
	//progs.at(currentProg)->setUniform("ViewProjectionMatrix", m);
	//progs.at(currentProg)->setUniform("MVP", m);
	//
	//// Render the quad
	//glBindVertexArray(bufferTextures.frame_Quad);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	
	// Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Blur pass one - read from blur texture one, blur it, write it to blur texture two
void SceneBasic_Uniform::drawPassThree()
{
	progs.at(currentProg)->setUniform("PassNo", 3);
	
	glBindFramebuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);
	// Prepare for rendering to HDR frame buffer
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	//glBindBuffer(GL_FRAMEBUFFER, bufferTextures.blur_FBO);
	//glBindBuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);

	
	// Set framebuffer colour data to hdr colour texture
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferTextures.hdr_Colour, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferTextures.blur_Two, 0);

	
	
	mat4 m = mat4(1.0f);
	progs.at(currentProg)->setUniform("NormalMatrix", m);
	progs.at(currentProg)->setUniform("ObjectModelMatrix", m);
	progs.at(currentProg)->setUniform("ModelMatrix", m);
	progs.at(currentProg)->setUniform("ModelViewMatrix", m);
	progs.at(currentProg)->setUniform("ViewMatrix", m);
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", m);
	progs.at(currentProg)->setUniform("MVP", m);
	setWeights();

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.blur_One);

	GLenum drawBuffers2[] = { GL_NONE, GL_NONE, GL_NONE, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(4, drawBuffers2);

	//// Set framebuffer colour data to blur two texture
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferTextures.blur_Two, 0);

	
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
void SceneBasic_Uniform::drawPassFour()
{
	progs.at(currentProg)->setUniform("PassNo", 4);
	
	glBindFramebuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);
	//GLfloat clearColour[] = {0.2f, 0.2f, 0.2f, 1.0f};
	//glClearBufferfv(GL_COLOR, 2, clearColour);
	// Prepare for rendering to HDR frame buffer
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glBindBuffer(GL_FRAMEBUFFER, bufferTextures.blur_FBO);
	//glBindBuffer(GL_FRAMEBUFFER, bufferTextures.hdr_FBO);

	// Set framebuffer colour data to blur one texture
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferTextures.blur_One, 0);
	
	mat4 m = mat4(1.0f);

	progs.at(currentProg)->setUniform("NormalMatrix", m);
	progs.at(currentProg)->setUniform("ObjectModelMatrix", m);
	progs.at(currentProg)->setUniform("ModelMatrix", m);
	progs.at(currentProg)->setUniform("ModelViewMatrix", m);
	progs.at(currentProg)->setUniform("ViewMatrix", m);
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", m);
	progs.at(currentProg)->setUniform("MVP", m);
	setWeights();

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, bufferTextures.blur_Two);
	
	GLenum drawBuffers2[] = { GL_NONE, GL_NONE, GL_COLOR_ATTACHMENT1, GL_NONE };
	glDrawBuffers(4, drawBuffers2);

	
	// Read from blur texture one, using linear sampling for extra blur
	glBindSampler(10, bufferTextures.linear_Sampler);

	// Render to the full screen quad
	glBindVertexArray(bufferTextures.frame_Quad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	// Revert to nearest sampling
	glBindSampler(10, bufferTextures.nearest_Sampler);

	//glClear(GL_COLOR_BUFFER_BIT);

	// Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Read from blur texture one and HDR texture, tone map the HDR and mix with blur
void SceneBasic_Uniform::drawPassFive()
{
	progs.at(currentProg)->setUniform("PassNo", 5);
	
	// Bind default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);

	//GLenum drawBuffers2[] = { GL_NONE, GL_NONE, GL_COLOR_ATTACHMENT1 };//, GL_COLOR_ATTACHMENT2 };
	//glDrawBuffers(3, drawBuffers2);

	// Prepare for rendering
	glClear(GL_COLOR_BUFFER_BIT);
	
	mat4 m = mat4(1.0f);

	progs.at(currentProg)->setUniform("NormalMatrix", m);
	progs.at(currentProg)->setUniform("ObjectModelMatrix", m);
	progs.at(currentProg)->setUniform("ModelMatrix", m);
	progs.at(currentProg)->setUniform("ModelViewMatrix", m);
	progs.at(currentProg)->setUniform("ViewMatrix", m);
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", m);
	progs.at(currentProg)->setUniform("MVP", m);

	
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

	
	//std::string currentShader = currentProg;
	//changeShader("Skybox Shader");
	//progs.at(currentProg)->setUniform("ViewProjectionMatrix", (cam->getProjection() * cam->getView() * skyboxRotate180Y));
	//skybox.render();
	//changeShader(currentShader);
}


void SceneBasic_Uniform::drawScene()
{
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


	////glDepthMask(GL_FALSE);
	changeShader("Skybox Shader");
	progs.at(currentProg)->setUniform("ViewProjectionMatrix", (cam->getProjection() * cam->getView() * skyboxRotate180Y));
	skybox.render();
	changeShader(currentShader);
	////glDepthMask(GL_TRUE);
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
		ImGui::Spacing();
		ImGui::Checkbox("Bloom effect", &doBloom);
		ImGui::Spacing();
		ImGui::SliderFloat("Skybox Brightness", &skyboxBrightness, 0.01f, 100.0f);
		ImGui::Spacing();
		ImGui::SliderFloat("HDR Exposure", &hdrExposure, 0.01f, 5.0f);
		ImGui::Separator();
		ImGui::SliderFloat("Bloom Threshold", &luminanceThreshold, 0.01f, 5.0f);
		ImGui::Separator();
		ImGui::SliderInt("Bloom axis level", &bloomAxisBlur, 1, 10);
		ImGui::Separator();
		ImGui::SliderInt("Bloom diagonal level", &bloomDiagonalBlur, 1, 10);
		ImGui::Separator();
		ImGui::Separator();
		//ImGui::SliderInt("Bloom Mipmap level", &bloomMipmapLevel, 0, 16);
		//ImGui::Spacing();

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

	if(currentProg == "PBR + HDR Shader")
	{
		drawPassOne();

		//drawScene();

		computeLogAvgLuminance();

		//drawPassTwo();

		drawPassThree();

		drawPassFour();

		drawPassFive();
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		drawScene();
	}
}


void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;

    glViewport(0,0,w,h);

	//projection = glm::perspective(glm::radians(70.0f), (float)w/h, 0.3f, 100.0f);
}


SceneBasic_Uniform::~SceneBasic_Uniform()
{
	glDeleteFramebuffers(1, &bufferTextures.blur_FBO);
	//glDeleteBuffers();
	glDeleteVertexArrays(1, &bufferTextures.frame_Quad);
	glDeleteSamplers(1, &bufferTextures.linear_Sampler);
	glDeleteSamplers(1, &bufferTextures.nearest_Sampler);

	glDeleteTextures(1, &bufferTextures.hdr_Colour);
	glDeleteTextures(1, &bufferTextures.blur_One);
	glDeleteTextures(1, &bufferTextures.blur_Two);

	glDeleteTextures(1, &textures.skybox_MountainLake);
	glDeleteTextures(1, &textures.skybox_PisaHDR);
	glDeleteTextures(1, &textures.brick_Albedo);
	glDeleteTextures(1, &textures.ogre_Albedo);
	glDeleteTextures(1, &textures.ogre_NormalMap);
	glDeleteTextures(1, &textures.ogre_AmbientOcclusionMap);
	glDeleteTextures(1, &textures.ripple_NormalMap);
	glDeleteTextures(1, &textures.wood_Albedo);
	glDeleteTextures(1, &textures.fire);
	glDeleteTextures(1, &textures.star);
	glDeleteTextures(1, &textures.cement);

	glDeleteTextures(1, &textures.alienMetal_Albedo);
	glDeleteTextures(1, &textures.alienMetal_Roughness);
	glDeleteTextures(1, &textures.alienMetal_Metallic);
	glDeleteTextures(1, &textures.alienMetal_NormalMap);
	glDeleteTextures(1, &textures.alienMetal_HeightMap);
	glDeleteTextures(1, &textures.alienMetal_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.copperScuffed_Albedo);
	glDeleteTextures(1, &textures.copperScuffed_Albedo_Boosted);
	glDeleteTextures(1, &textures.copperScuffed_Roughness);
	glDeleteTextures(1, &textures.copperScuffed_Metallic);
	glDeleteTextures(1, &textures.copperScuffed_NormalMap);
	glDeleteTextures(1, &textures.copperScuffed_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.rustedIron_Albedo);
	glDeleteTextures(1, &textures.rustedIron_Roughness);
	glDeleteTextures(1, &textures.rustedIron_Metallic);
	glDeleteTextures(1, &textures.rustedIron_NormalMap);
	glDeleteTextures(1, &textures.rustedIron_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.bambooWood_Albedo);
	glDeleteTextures(1, &textures.bambooWood_Roughness);
	glDeleteTextures(1, &textures.bambooWood_Metallic);
	glDeleteTextures(1, &textures.bambooWood_NormalMap);
	glDeleteTextures(1, &textures.bambooWood_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.redBrick_Albedo);
	glDeleteTextures(1, &textures.redBrick_Roughness);
	glDeleteTextures(1, &textures.redBrick_Metallic);
	glDeleteTextures(1, &textures.redBrick_NormalMap);
	glDeleteTextures(1, &textures.redBrick_HeightMap);
	glDeleteTextures(1, &textures.redBrick_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.grayGraniteFlecks_Albedo);
	glDeleteTextures(1, &textures.grayGraniteFlecks_Roughness);
	glDeleteTextures(1, &textures.grayGraniteFlecks_Metallic);
	glDeleteTextures(1, &textures.grayGraniteFlecks_NormalMap);
	glDeleteTextures(1, &textures.grayGraniteFlecks_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.scuffedPlastic_Albedo);
	glDeleteTextures(1, &textures.scuffedPlastic_Roughness);
	glDeleteTextures(1, &textures.scuffedPlastic_Metallic);
	glDeleteTextures(1, &textures.scuffedPlastic_NormalMap);
	glDeleteTextures(1, &textures.scuffedPlastic_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.humanSkin_Albedo);
	glDeleteTextures(1, &textures.humanSkin_Roughness);
	glDeleteTextures(1, &textures.humanSkin_Metallic);
	glDeleteTextures(1, &textures.humanSkin_NormalMap);
	glDeleteTextures(1, &textures.humanSkin_HeightMap);
	glDeleteTextures(1, &textures.humanSkin_AmbientOcclusionMap);

	glDeleteTextures(1, &textures.skybox_ArchesE_PineTree);
	glDeleteTextures(1, &textures.skybox_env_ArchesE_PineTree);
	glDeleteTextures(1, &textures.skybox_SummiPool);
	glDeleteTextures(1, &textures.skybox_env_SummiPool);

}