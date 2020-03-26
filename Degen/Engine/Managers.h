#pragma once
#include "VAO and Meshes/cVAOManager.h"
#include "Shaders/cShaderManager.h"
#include "FileReading/cModelLoader.h"
#include "Entity/cEntityManager.h"


namespace Degen
{
	static VAOAndModel::cVAOManager* VAOManager; // Manages objects pushed to gpu
	static Shaders::cShaderManager* ShaderManager; // managers all shaders created
	static FileReading::cModelLoader* ModelLoader; // manages files loded with assimp
	static Entity::cEntityManager* EntityManager; // creats and cleans up entities
	
}
