#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

//#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>

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

class SceneBasic_Uniform : public Scene
{
private:
    //GLuint currentProg = 0;
    std::string currentProg;
    //GLSLProgram prog;
    std::map<std::string, std::unique_ptr<GLSLProgram>> progs;

	std::vector<LightInfo> lights;

    SkyBox skybox;
    Plane floor;
    Cube metalCube;
    Cube box;
    Torus torus;
    Teapot teapot;
    std::unique_ptr<ObjMesh> piggy;
    std::unique_ptr<ObjMesh> ogre;

    void compile();
    void setMatrices();
    void setMeshUniforms(TriangleMesh* mesh);
	void setLights();
    void drawGUI();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void changeShader(std::string shaderName);

    virtual ~SceneBasic_Uniform() {};

private:
    struct Textures
	{

		GLuint skybox_MountainLake =		Texture::loadCubeMap("media/texture/skybox/lake180", ".jpg");
		//GLuint skybox_PisaHDR =				Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr");

		GLuint brick_Albedo =				Texture::loadTexture("media/texture/brick/brick1.jpg");
		//GLuint brick_NormalMap =			Texture::loadTexture("media/texture/brick/NormalMap.png");
		//GLuint brick_DisplacementMap =		Texture::loadTexture("media/texture/brick/DisplacementMap.png");
		//GLuint brick_AmbientOcclusionMap =  Texture::loadTexture("media/texture/brick/AmbientOcclusionMap.png");
		//GLuint brick_SpecularMap =			Texture::loadTexture("media/texture/brick/SpecularMap.png");

		GLuint ogre_Albedo =				Texture::loadTexture("media/texture/ogre/ogre_diffuse.png");
		GLuint ogre_NormalMap =				Texture::loadTexture("media/texture/ogre/ogre_normalmap.png");
		//GLuint ogre_AmbientOcclusionMap =	Texture::loadTexture("media/texture/ogre/ao_ears.png");
		
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

		GLuint alienMetal_Albedo = Texture::loadTexture("media/3rd_party/alien-metal/alien-metal_albedo.png");
		GLuint alienMetal_Roughness = Texture::loadTexture("media/3rd_party/alien-metal/alien-metal_roughness.png");
		GLuint alienMetal_Metallic = Texture::loadTexture("media/3rd_party/alien-metal/alien-metal_metallic.png");
		GLuint alienMetal_NormalMap = Texture::loadTexture("media/3rd_party/alien-metal/alien-metal_normal.png");
		GLuint alienMetal_HeightMap = Texture::loadTexture("media/3rd_party/alien-metal/alien-metal_height.png");
		GLuint alienMetal_AmbientOcclusionMap = Texture::loadTexture("media/3rd_party/alien-metal/alien-metal_ao.png");

		GLuint aluminumScuffed_Albedo = Texture::loadTexture("media/3rd_party/aluminum-scuffed/aluminum-scuffed_albedo.png");
		GLuint aluminumScuffed_Roughness = Texture::loadTexture("media/3rd_party/aluminum-scuffed/aluminum-scuffed_roughness.png");
		GLuint aluminumScuffed_Metallic = Texture::loadTexture("media/3rd_party/aluminum-scuffed/aluminum-scuffed_metallic.png");
		GLuint aluminumScuffed_NormalMap = Texture::loadTexture("media/3rd_party/aluminum-scuffed/aluminum-scuffed_normal.png");
		////GLuint aluminumScuffed_HeightMap = Texture::loadTexture("media/3rd_party/aluminum-scuffed/aluminum-scuffed_height.png");
		////GLuint aluminumScuffed_AmbientOcclusionMap = Texture::loadTexture("media/3rd_party/aluminum-scuffed/aluminum-scuffed_ao.png");
		
		GLuint bambooWood_Albedo = Texture::loadTexture("media/3rd_party/bamboo-wood-semigloss/bamboo-wood-semigloss_albedo.png");
		GLuint bambooWood_Roughness = Texture::loadTexture("media/3rd_party/bamboo-wood-semigloss/bamboo-wood-semigloss_roughness.png");
		GLuint bambooWood_Metallic = Texture::loadTexture("media/3rd_party/bamboo-wood-semigloss/bamboo-wood-semigloss_metallic.png");
		GLuint bambooWood_NormalMap = Texture::loadTexture("media/3rd_party/bamboo-wood-semigloss/bamboo-wood-semigloss_normal.png");
		////GLuint bambooWood_HeightMap = Texture::loadTexture("media/3rd_party/bamboo-wood-semigloss/bamboo-wood-semigloss_height.png");
		GLuint bambooWood_AmbientOcclusionMap = Texture::loadTexture("media/3rd_party/bamboo-wood-semigloss/bamboo-wood-semigloss_ao.png");

    	GLuint copperScuffed_Albedo = Texture::loadTexture("media/3rd_party/copper-scuffed/copper-scuffed_albedo.png");
    	GLuint copperScuffedBoosted_Albedo = Texture::loadTexture("media/3rd_party/copper-scuffed/copper-scuffed_albedo_boosted.png");
		GLuint copperScuffed_Roughness = Texture::loadTexture("media/3rd_party/copper-scuffed/copper-scuffed_roughness.png");
		GLuint copperScuffed_Metallic = Texture::loadTexture("media/3rd_party/copper-scuffed/copper-scuffed_metallic.png");
		GLuint copperScuffed_NormalMap = Texture::loadTexture("media/3rd_party/copper-scuffed/copper-scuffed_normal.png");
		////GLuint copperScuffed_HeightMap = Texture::loadTexture("media/3rd_party/copper-scuffed/copper-scuffed_height.png");
		////GLuint copperScuffed_AmbientOcclusionMap = Texture::loadTexture("media/3rd_party/copper-scuffed/copper-scuffed_ao.png");
		
		GLuint grayGraniteFlecks_Albedo = Texture::loadTexture("media/3rd_party/gray-granite-flecks/gray-granite-flecks_albedo.png");
		GLuint grayGraniteFlecks_Roughness = Texture::loadTexture("media/3rd_party/gray-granite-flecks/gray-granite-flecks_roughness.png");
		GLuint grayGraniteFlecks_Metallic = Texture::loadTexture("media/3rd_party/gray-granite-flecks/gray-granite-flecks_metallic.png");
		GLuint grayGraniteFlecks_NormalMap = Texture::loadTexture("media/3rd_party/gray-granite-flecks/gray-granite-flecks_normal.png");
		////GLuint grayGraniteFlecks_HeightMap = Texture::loadTexture("media/3rd_party/gray-granite-flecks/gray-granite-flecks_height.png");
		GLuint grayGraniteFlecks_AmbientOcclusionMap = Texture::loadTexture("media/3rd_party/gray-granite-flecks/gray-granite-flecks_ao.png");
		
    	GLuint humanSkin_Albedo = Texture::loadTexture("media/3rd_party/human-skin/human-skin_albedo_1.png");
		GLuint humanSkin_Roughness = Texture::loadTexture("media/3rd_party/human-skin/human-skin_roughness.png");
		GLuint humanSkin_Metallic = Texture::loadTexture("media/3rd_party/human-skin/human-skin_metallic.png");
		GLuint humanSkin_NormalMap = Texture::loadTexture("media/3rd_party/human-skin/human-skin_normal.png");
		GLuint humanSkin_HeightMap = Texture::loadTexture("media/3rd_party/human-skin/human-skin_height.png");
		GLuint humanSkin_AmbientOcclusionMap = Texture::loadTexture("media/3rd_party/human-skin/human-skin_ao.png");
		
		GLuint rustedIron_Albedo = Texture::loadTexture("media/3rd_party/rusted-iron/rusted-iron_albedo.png");
		GLuint rustedIron_Roughness = Texture::loadTexture("media/3rd_party/rusted-iron/rusted-iron_roughness.png");
		GLuint rustedIron_Metallic = Texture::loadTexture("media/3rd_party/rusted-iron/rusted-iron_metallic.png");
		GLuint rustedIron_NormalMap = Texture::loadTexture("media/3rd_party/rusted-iron/rusted-iron_normal.png");
		////GLuint rustedIron_HeightMap = Texture::loadTexture("media/3rd_party/rusted-iron/rusted-iron_height.png");
		////GLuint rustedIron_AmbientOcclusionMap = Texture::loadTexture("media/3rd_party/rusted-iron/rusted-iron_ao.png");
		
		GLuint scuffedPlastic_Albedo = Texture::loadTexture("media/3rd_party/scuffed-plastic/scuffed-plastic_albedo_1.png");
		GLuint scuffedPlastic_Roughness = Texture::loadTexture("media/3rd_party/scuffed-plastic/scuffed-plastic_roughness.png");
		GLuint scuffedPlastic_Metallic = Texture::loadTexture("media/3rd_party/scuffed-plastic/scuffed-plastic_metallic.png");
		GLuint scuffedPlastic_NormalMap = Texture::loadTexture("media/3rd_party/scuffed-plastic/scuffed-plastic_normal.png");
		////GLuint scuffedPlastic_HeightMap = Texture::loadTexture("media/3rd_party/scuffed-plastic/scuffed-plastic_height.png");
		GLuint scuffedPlastic_AmbientOcclusionMap = Texture::loadTexture("media/3rd_party/scuffed-plastic/scuffed-plastic_ao.png");
		
		GLuint titaniumScuffed_Albedo = Texture::loadTexture("media/3rd_party/titanium-scuffed/titanium-scuffed_albedo.png");
		GLuint titaniumScuffed_Roughness = Texture::loadTexture("media/3rd_party/titanium-scuffed/titanium-scuffed_roughness.png");
		GLuint titaniumScuffed_Metallic = Texture::loadTexture("media/3rd_party/titanium-scuffed/titanium-scuffed_metallic.png");
		GLuint titaniumScuffed_NormalMap = Texture::loadTexture("media/3rd_party/titanium-scuffed/titanium-scuffed_normal.png");
		////GLuint titaniumScuffed_HeightMap = Texture::loadTexture("media/3rd_party/titanium-scuffed/titanium-scuffed_height.png");
		////GLuint titaniumScuffed_AmbientOcclusionMap = Texture::loadTexture("media/3rd_party/titanium-scuffed/titanium-scuffed_ao.png");

	} textures;
};

#endif // SCENEBASIC_UNIFORM_H
