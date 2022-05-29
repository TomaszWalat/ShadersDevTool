#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include <glm/glm.hpp>

#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/texture.h"

#include "lightInfo.h"
#include "model/cube.h"
#include "model/plane.h"
#include "model/torus.h"
#include "model/teapot.h"
#include "model/objmesh.h"
#include "model/skybox.h"
#include "model/sphere.h"

class SceneBasic_Uniform : public Scene
{
private:
    std::string currentProg;
    std::map<std::string, std::unique_ptr<GLSLProgram>> progs; // Stores the various shader programs compiled by compile()
	
	std::vector<LightInfo> lights; // Stores scene lights

	bool usePBRTextures = true; // Indicates if the texture or user input based PBR shader is used

	float gammaCorrection = 2.2f; // Gamma correction value
	
	bool doHDRToneMapping = true; // HDR on/off
	float hdrExposure = 0.12f;

	bool doBloom = true; // Bloom on/off
	float weights[10], sum, sigma2 = 25.0f; // for calculating gaussian blur weights
	float luminanceThreshold = 1.7f; // bloom brightness threshold
	int bloomBlurOneStrength = 6;	// strength of first blur pass
	int bloomBlurTwoStrength = 5;	// strength of second blur pass
	

	// For changing skybox orientation
	glm::mat4 skyboxRotate180Y = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// --- Models --- //
    SkyBox skybox;
	Sphere lightSphere;

	// show floor 1 (in the direction of negative Z)
    Plane floor_1;
    Cube metalCube;
    Cube box;
    Torus torus;
    Teapot teapot;
    std::unique_ptr<ObjMesh> piggy;
    std::unique_ptr<ObjMesh> ogre;
	
	// show floor 2 (in the direction of positive X)
    Plane floor_2; 
	Sphere ball_1;
	Sphere ball_2;
	Sphere ball_3;
	Sphere ball_4;
	Sphere ball_5;
	Sphere ball_6;
	Sphere ball_7;
	Sphere ball_8;
	Sphere ball_9;
	Sphere ball_10;
	Sphere ball_11;
	Sphere ball_12;
	Sphere ball_13;
	Sphere ball_14;
	Sphere ball_15;
	Sphere ball_16;
	Sphere ball_17;
	Sphere ball_18;
	Sphere ball_19;
	Sphere ball_20;
	Sphere ball_21;
	Sphere ball_22;
	Sphere ball_23;
	Sphere ball_24;
	Sphere ball_25;

	// show floor 3 (in the direction of positive Z)
    Plane floor_3;
	std::unique_ptr<ObjMesh> cow_1;
	std::unique_ptr<ObjMesh> cow_2;
	std::unique_ptr<ObjMesh> cow_3;
	std::unique_ptr<ObjMesh> cow_4;
	std::unique_ptr<ObjMesh> cow_5;

	//// show floor 4 (in the direction of negative X)
 //   Plane floor_4;

    void compile(); // Assembles and compiles shader programs from "shaders" vector

	// Bloom / blur functions
	void setupFBO();
	void setupFrameQuad();
	void setupSamplers();
	void computeWeights();
	float gauss(float x, float sigma2);
	void computeLogAvgLuminance();

	// Shader uniform setting functions
    void setMatrices();
    void setMeshUniforms(TriangleMesh* mesh);
	void setLights();
	void setWeights();

	// Drawing functions (PBR, HDR, Bloom)
	void drawPassOne();
	void drawPassTwo();
	void drawPassThree();
	void drawPassFour();

	// Core drawing functions
	void drawScene();
    void drawGUI();

public:
    SceneBasic_Uniform();

    void initScene();

    void changeShader(std::string shaderName); // Function for dynamically changing shaders 

    void render(); // Render function - calls draw functions

    void update( float t ); // Game loop / time update - for animations and/or physics

    void resize(int, int); // Resize viewport

    ~SceneBasic_Uniform();

private:
	// HDR and Bloom processing
	struct BufferTextures
	{
		GLuint frame_Quad;
		GLuint hdr_FBO;
		GLuint hdr_Colour, blur_One, blur_Two;
		GLuint linear_Sampler, nearest_Sampler;
	}bufferTextures;

	// General texture storage
    struct Textures
	{
		// Practical and/or generated textures

		GLuint skybox_MountainLake =		Texture::loadCubeMap("media/texture/skybox/lake180", ".jpg");
		GLuint skybox_PisaHDR =				Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");

		GLuint brick_Albedo =				Texture::loadTexture("media/texture/brick/brick1.jpg");
		//GLuint brick_NormalMap =			Texture::loadTexture("media/texture/brick/NormalMap.png");
		//GLuint brick_DisplacementMap =		Texture::loadTexture("media/texture/brick/DisplacementMap.png");
		//GLuint brick_AmbientOcclusionMap =  Texture::loadTexture("media/texture/brick/AmbientOcclusionMap.png");
		//GLuint brick_SpecularMap =			Texture::loadTexture("media/texture/brick/SpecularMap.png");

		GLuint ogre_Albedo =				Texture::loadTexture("media/texture/ogre/ogre_diffuse.png");
		GLuint ogre_NormalMap =				Texture::loadTexture("media/texture/ogre/ogre_normalmap.png");
		GLuint ogre_AmbientOcclusionMap =	Texture::loadTexture("media/texture/ogre/ao_ears.png");
		
		//GLuint ripple_Albedo =				Texture::loadTexture("media/texture/ripple/ ---");
		GLuint ripple_NormalMap =			Texture::loadTexture("media/texture/ripple/NormalMap_invertedR.png");
		//GLuint ripple_DisplacementMap =		Texture::loadTexture("media/texture/ripple/DisplacementMap.png");
		//GLuint ripple_AmbientOcclusionMap = Texture::loadTexture("media/texture/ripple/AmbientOcclusionMap.png");
		//GLuint ripple_SpecularMap =			Texture::loadTexture("media/texture/ripple/SpecularMap.png");

		GLuint wood_Albedo =				Texture::loadTexture("media/texture/wood/Albedo.jpg");
		//GLuint wood_Roughness =				Texture::loadTexture("media/texture/wood/Roughness.jpg");
		//GLuint wood_NormalMap =				Texture::loadTexture("media/texture/wood/NormalMap.png");
		//GLuint wood_DisplacementMap =		Texture::loadTexture("media/texture/wood/DisplacementMap.png");
		//GLuint wood_AmbientOcclusionMap =	Texture::loadTexture("media/texture/wood/AmbientOcclusionMap.png");
		//GLuint wood_SpecularMap =			Texture::loadTexture("media/texture/wood/SpecularMap.png");

		//GLuint blueWater =					Texture::loadTexture("media/texture/bluewater.png");
		GLuint fire =						Texture::loadTexture("media/texture/fire.png");
		//GLuint smoke =						Texture::loadTexture("media/texture/smoke.png");
		GLuint star =						Texture::loadTexture("media/texture/star.png");

		GLuint cement =						Texture::loadTexture("media/texture/cement.jpg");
		//GLuint flower =						Texture::loadTexture("media/texture/flower.png");
		//GLuint textile =					Texture::loadTexture("media/texture/textile.png");
		//GLuint moss =						Texture::loadTexture("media/texture/moss.png");
		//GLuint spotCow_Albedo =				Texture::loadTexture("media/texture/spot_texture.png");


		// --- 3rd party textures from freepbr.com --- //

		std::string groupPath_pbr = "media/3rd_party/materialTextures-pbr/";

		GLuint alienMetal_Albedo =						Texture::loadTexture(groupPath_pbr + "alien-metal/alien-metal_albedo.png");
		GLuint alienMetal_Roughness =					Texture::loadTexture(groupPath_pbr + "alien-metal/alien-metal_roughness.png");
		GLuint alienMetal_Metallic =					Texture::loadTexture(groupPath_pbr + "alien-metal/alien-metal_metallic.png");
		GLuint alienMetal_NormalMap =					Texture::loadTexture(groupPath_pbr + "alien-metal/alien-metal_normal.png");
		GLuint alienMetal_HeightMap =					Texture::loadTexture(groupPath_pbr + "alien-metal/alien-metal_height.png");
		GLuint alienMetal_AmbientOcclusionMap =			Texture::loadTexture(groupPath_pbr + "alien-metal/alien-metal_ao.png");

		//GLuint aluminumScuffed_Albedo =					Texture::loadTexture(groupPath_pbr + "aluminum-scuffed/aluminum-scuffed_albedo.png");
		//GLuint aluminumScuffed_Roughness =				Texture::loadTexture(groupPath_pbr + "aluminum-scuffed/aluminum-scuffed_roughness.png");
		//GLuint aluminumScuffed_Metallic =				Texture::loadTexture(groupPath_pbr + "aluminum-scuffed/aluminum-scuffed_metallic.png");
		//GLuint aluminumScuffed_NormalMap =				Texture::loadTexture(groupPath_pbr + "aluminum-scuffed/aluminum-scuffed_normal.png");
		//////GLuint aluminumScuffed_HeightMap =				Texture::loadTexture(groupPath_pbr + "aluminum-scuffed/aluminum-scuffed_height.png");
		//GLuint aluminumScuffed_AmbientOcclusionMap =	Texture::loadTexture(groupPath_pbr + "aluminum-scuffed/aluminum-scuffed_ao.png");
		
		//GLuint titaniumScuffed_Albedo =					Texture::loadTexture(groupPath_pbr + "titanium-scuffed/titanium-scuffed_albedo.png");
		//GLuint titaniumScuffed_Roughness =				Texture::loadTexture(groupPath_pbr + "titanium-scuffed/titanium-scuffed_roughness.png");
		//GLuint titaniumScuffed_Metallic =				Texture::loadTexture(groupPath_pbr + "titanium-scuffed/titanium-scuffed_metallic.png");
		//GLuint titaniumScuffed_NormalMap =				Texture::loadTexture(groupPath_pbr + "titanium-scuffed/titanium-scuffed_normal.png");
		//////GLuint titaniumScuffed_HeightMap =				Texture::loadTexture(groupPath_pbr + "titanium-scuffed/titanium-scuffed_height.png");
		//GLuint titaniumScuffed_AmbientOcclusionMap =	Texture::loadTexture(groupPath_pbr + "titanium-scuffed/titanium-scuffed_ao.png");

    	GLuint copperScuffed_Albedo =					Texture::loadTexture(groupPath_pbr + "copper-scuffed/copper-scuffed_albedo.png");
    	GLuint copperScuffed_Albedo_Boosted =			Texture::loadTexture(groupPath_pbr + "copper-scuffed/copper-scuffed_albedo-boosted.png");
		GLuint copperScuffed_Roughness =				Texture::loadTexture(groupPath_pbr + "copper-scuffed/copper-scuffed_roughness.png");
		GLuint copperScuffed_Metallic =					Texture::loadTexture(groupPath_pbr + "copper-scuffed/copper-scuffed_metallic.png");
		GLuint copperScuffed_NormalMap =				Texture::loadTexture(groupPath_pbr + "copper-scuffed/copper-scuffed_normal.png");
		////GLuint copperScuffed_HeightMap =				Texture::loadTexture(groupPath_pbr + "copper-scuffed/copper-scuffed_height.png");
		GLuint copperScuffed_AmbientOcclusionMap =		Texture::loadTexture(groupPath_pbr + "copper-scuffed/copper-scuffed_ao.png");
		
		GLuint rustedIron_Albedo =						Texture::loadTexture(groupPath_pbr + "rusted-iron/rusted-iron_albedo.png");
		GLuint rustedIron_Roughness =					Texture::loadTexture(groupPath_pbr + "rusted-iron/rusted-iron_roughness.png");
		GLuint rustedIron_Metallic =					Texture::loadTexture(groupPath_pbr + "rusted-iron/rusted-iron_metallic.png");
		GLuint rustedIron_NormalMap =					Texture::loadTexture(groupPath_pbr + "rusted-iron/rusted-iron_normal.png");
		////GLuint rustedIron_HeightMap =					Texture::loadTexture(groupPath_pbr + "rusted-iron/rusted-iron_height.png");
		GLuint rustedIron_AmbientOcclusionMap =			Texture::loadTexture(groupPath_pbr + "rusted-iron/rusted-iron_ao.png");
		
		GLuint bambooWood_Albedo =						Texture::loadTexture(groupPath_pbr + "bamboo-wood-semigloss/bamboo-wood-semigloss_albedo.png");
		GLuint bambooWood_Roughness =					Texture::loadTexture(groupPath_pbr + "bamboo-wood-semigloss/bamboo-wood-semigloss_roughness.png");
		GLuint bambooWood_Metallic =					Texture::loadTexture(groupPath_pbr + "bamboo-wood-semigloss/bamboo-wood-semigloss_metallic.png");
		GLuint bambooWood_NormalMap =					Texture::loadTexture(groupPath_pbr + "bamboo-wood-semigloss/bamboo-wood-semigloss_normal.png");
		////GLuint bambooWood_HeightMap =					Texture::loadTexture(groupPath_pbr + "bamboo-wood-semigloss/bamboo-wood-semigloss_height.png");
		GLuint bambooWood_AmbientOcclusionMap =			Texture::loadTexture(groupPath_pbr + "bamboo-wood-semigloss/bamboo-wood-semigloss_ao.png");

		GLuint mahoganyFloor_Albedo =					Texture::loadTexture(groupPath_pbr + "mahogany-floor/mahogany-floor_albedo.png");
		GLuint mahoganyFloor_Roughness =				Texture::loadTexture(groupPath_pbr + "mahogany-floor/mahogany-floor_roughness.png");
		GLuint mahoganyFloor_Metallic =					Texture::loadTexture(groupPath_pbr + "mahogany-floor/mahogany-floor_metallic.png");
		GLuint mahoganyFloor_NormalMap =				Texture::loadTexture(groupPath_pbr + "mahogany-floor/mahogany-floor_normal.png");
		GLuint mahoganyFloor_HeightMap =				Texture::loadTexture(groupPath_pbr + "mahogany-floor/mahogany-floor_height.png");
		GLuint mahoganyFloor_AmbientOcclusionMap =		Texture::loadTexture(groupPath_pbr + "mahogany-floor/mahogany-floor_ao.png");

		GLuint patchyCement_Albedo =					Texture::loadTexture(groupPath_pbr + "patchy-cement/patchy-cement_albedo.png");
		GLuint patchyCement_Roughness =					Texture::loadTexture(groupPath_pbr + "patchy-cement/patchy-cement_roughness.png");
		GLuint patchyCement_Metallic =					Texture::loadTexture(groupPath_pbr + "patchy-cement/patchy-cement_metallic.png");
		GLuint patchyCement_NormalMap =					Texture::loadTexture(groupPath_pbr + "patchy-cement/patchy-cement_normal.png");
		////GLuint patchyCement_HeightMap =					Texture::loadTexture(groupPath_pbr + "patchy-cement/patchy-cement_height.png");
		GLuint patchyCement_AmbientOcclusionMap =		Texture::loadTexture(groupPath_pbr + "patchy-cement/patchy-cement_ao.png");

		GLuint redBrick_Albedo =						Texture::loadTexture(groupPath_pbr + "red-bricks/red-bricks_albedo.png");
		GLuint redBrick_Roughness =						Texture::loadTexture(groupPath_pbr + "red-bricks/red-bricks_roughness.png");
		GLuint redBrick_Metallic =						Texture::loadTexture(groupPath_pbr + "red-bricks/red-bricks_metallic.png");
		GLuint redBrick_NormalMap =						Texture::loadTexture(groupPath_pbr + "red-bricks/red-bricks_normal.png");
		GLuint redBrick_HeightMap =						Texture::loadTexture(groupPath_pbr + "red-bricks/red-bricks_height.png");
		GLuint redBrick_AmbientOcclusionMap =			Texture::loadTexture(groupPath_pbr + "red-bricks/red-bricks_ao.png");

		GLuint wornPaintedCement_Albedo =				Texture::loadTexture(groupPath_pbr + "worn-painted-cement/worn-painted-cement_albedo.png");
		GLuint wornPaintedCement_Roughness =			Texture::loadTexture(groupPath_pbr + "worn-painted-cement/worn-painted-cement_roughness.png");
		GLuint wornPaintedCement_Metallic =				Texture::loadTexture(groupPath_pbr + "worn-painted-cement/worn-painted-cement_metallic.png");
		GLuint wornPaintedCement_NormalMap =			Texture::loadTexture(groupPath_pbr + "worn-painted-cement/worn-painted-cement_normal.png");
		////GLuint wornPaintedCement_HeightMap =			Texture::loadTexture(groupPath_pbr + "worn-painted-cement/worn-painted-cement_height.png");
		GLuint wornPaintedCement_AmbientOcclusionMap =	Texture::loadTexture(groupPath_pbr + "worn-painted-cement/worn-painted-cement_ao.png");

		GLuint grayGraniteFlecks_Albedo =				Texture::loadTexture(groupPath_pbr + "gray-granite-flecks/gray-granite-flecks_albedo.png");
		GLuint grayGraniteFlecks_Roughness =			Texture::loadTexture(groupPath_pbr + "gray-granite-flecks/gray-granite-flecks_roughness.png");
		GLuint grayGraniteFlecks_Metallic =				Texture::loadTexture(groupPath_pbr + "gray-granite-flecks/gray-granite-flecks_metallic.png");
		GLuint grayGraniteFlecks_NormalMap =			Texture::loadTexture(groupPath_pbr + "gray-granite-flecks/gray-granite-flecks_normal.png");
		////GLuint grayGraniteFlecks_HeightMap =			Texture::loadTexture(groupPath_pbr + "gray-granite-flecks/gray-granite-flecks_height.png");
		GLuint grayGraniteFlecks_AmbientOcclusionMap =	Texture::loadTexture(groupPath_pbr + "gray-granite-flecks/gray-granite-flecks_ao.png");
		
		GLuint scuffedPlastic_Albedo =					Texture::loadTexture(groupPath_pbr + "scuffed-plastic/scuffed-plastic_albedo_1.png");
		GLuint scuffedPlastic_Roughness =				Texture::loadTexture(groupPath_pbr + "scuffed-plastic/scuffed-plastic_roughness.png");
		GLuint scuffedPlastic_Metallic =				Texture::loadTexture(groupPath_pbr + "scuffed-plastic/scuffed-plastic_metallic.png");
		GLuint scuffedPlastic_NormalMap =				Texture::loadTexture(groupPath_pbr + "scuffed-plastic/scuffed-plastic_normal.png");
		////GLuint scuffedPlastic_HeightMap =				Texture::loadTexture(groupPath_pbr + "scuffed-plastic/scuffed-plastic_height.png");
		GLuint scuffedPlastic_AmbientOcclusionMap =		Texture::loadTexture(groupPath_pbr + "scuffed-plastic/scuffed-plastic_ao.png");
		
    	GLuint humanSkin_Albedo =						Texture::loadTexture(groupPath_pbr + "human-skin/human-skin_albedo_1.png");
		GLuint humanSkin_Roughness =					Texture::loadTexture(groupPath_pbr + "human-skin/human-skin_roughness.png");
		GLuint humanSkin_Metallic =						Texture::loadTexture(groupPath_pbr + "human-skin/human-skin_metallic.png");
		GLuint humanSkin_NormalMap =					Texture::loadTexture(groupPath_pbr + "human-skin/human-skin_normal.png");
		GLuint humanSkin_HeightMap =					Texture::loadTexture(groupPath_pbr + "human-skin/human-skin_height.png");
		GLuint humanSkin_AmbientOcclusionMap =			Texture::loadTexture(groupPath_pbr + "human-skin/human-skin_ao.png");

		// --- 3rd party textures from hdrlabs.com --- //

		std::string groupPath_hdr = "media/3rd_party/cubemaps-hdr/";

		GLuint skybox_ArchesE_PineTree =	Texture::loadHdrCubeMap(groupPath_hdr + "Arches_E_PineTree/arches");
		GLuint skybox_env_ArchesE_PineTree =	Texture::loadHdrCubeMap(groupPath_hdr + "Arches_E_PineTree/arches_env");
		//GLuint skybox_IceLake =				Texture::loadHdrCubeMap(groupPath_hdr + "media/texture/skybox/lake180");
		//GLuint skybox_Milkyway =			Texture::loadHdrCubeMap(groupPath_hdr + "media/texture/skybox/lake180");
		//GLuint skybox_MonoLakeC =			Texture::loadHdrCubeMap(groupPath_hdr + "media/texture/skybox/lake180");
		GLuint skybox_SummiPool =			Texture::loadHdrCubeMap(groupPath_hdr + "Summi_Pool/summi");
		GLuint skybox_env_SummiPool =			Texture::loadHdrCubeMap(groupPath_hdr + "Summi_Pool/summi_env");


	} textures;
};

#endif // SCENEBASIC_UNIFORM_H
