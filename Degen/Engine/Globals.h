#pragma once
#include "Entity/cEntityManager.h"
#include "FileReading/cModelLoader.h"
#include "Shaders/cShaderManager.h"
#include "VAO and Meshes/cVAOManager.h"
#include "sView.h"
#include "Texture/cTextureManager.h"

namespace Degen
{
	extern int WINDOW_WIDTH;
	extern int WINDOW_HEIGHT;

	extern VAOAndModel::cVAOManager* VAOManager; // Manages objects pushed to gpu
	extern Shaders::cShaderManager* ShaderManager; // managers all shaders created
	extern FileReading::cModelLoader* ModelLoader; // manages files loded with assimp
	extern Entity::cEntityManager* EntityManager; // creats and cleans up entities
	extern sView* View; // view for rendering updated by camera
	extern Texture::cTextureManager* TextureManager;
}
