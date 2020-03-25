#pragma once
#include "VAO and Meshes/cVAOManager.h"
#include "Shaders/cShaderManager.h"
#include "FileReading/cModelLoader.h"


namespace Degen
{
	static VAOAndModel::cVAOManager* VAOManager;
	static Shaders::cShaderManager* ShaderManager;
	static FileReading::cModelLoader* ModelLoader;
}
