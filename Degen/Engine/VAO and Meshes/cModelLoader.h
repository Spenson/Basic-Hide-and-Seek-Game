#pragma once
#include <string>
#include <map>

#include <assimp/Importer.hpp>		// To load from a file
#include "sModelDrawInfo.h"

namespace Degen
{
	namespace VAOAndModel
	{
		class cModelLoader
		{
			std::map<std::string, Assimp::Importer> mImporters;
		public:
			cModelLoader();
			~cModelLoader();
			// Imports all meshes in file then abandons Importer
			sModelDrawInfo* LoadBasicModel(std::string	file_name, std::string friendly_name, std::string& error);
		};

		
	}
}
