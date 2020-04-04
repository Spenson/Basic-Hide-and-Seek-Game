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
			std::string base_path = "assets/models/";
		public:
			cModelLoader() = default;
			~cModelLoader() = default;

			VAOAndModel::sModelDrawInfo* LoadBasicModel(std::string	file_name, std::string friendly_name, std::string& error, const unsigned int meshIndex = 0);
			VAOAndModel::sModelDrawInfo* LoadModel(std::string	file_name, std::string friendly_name, std::string& error, const unsigned int meshIndex = 0);


			
			// Previous testing
			VAOAndModel::sModelDrawInfo* LoadPlyModel(std::string file_name, std::string friendly_name, std::string& error);
		};

		
	}
}
