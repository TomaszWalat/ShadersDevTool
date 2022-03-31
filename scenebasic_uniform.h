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

	} textures;
};

#endif // SCENEBASIC_UNIFORM_H
