#pragma once
#include <string>
#include <map>

#include <assimp/Importer.hpp>		// To load from a file
#include "../VAO and Meshes/sModelDrawInfo.h"

namespace Degen
{
	namespace FileReading
	{
		class cModelLoader
		{
			std::map<std::string, Assimp::Importer> mImporters;
		public:
			cModelLoader();
			~cModelLoader();
			// Imports all meshes in file then abandons Importer
			VAOAndModel::sModelDrawInfo* LoadBasicModel(std::string	file_name, std::string friendly_name, std::string& error);
		};

		
	}
}
