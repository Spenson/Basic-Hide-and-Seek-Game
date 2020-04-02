#pragma once
#include <map>
#include "sTextureInfo.h"


namespace Degen
{
	namespace Texture
	{
		class cTextureManager
		{
		public:
			bool Create2DTextureFromBMPFile(std::string textureFileName, bool bGenerateMIPMap);
			bool Create2DTextureFromPNGFile(std::string textureFileName, std::string friendly_name, bool bGenerateMIPMap = true);

			bool CreateCubeTextureFromBMPFiles(std::string cubeMapName,
											   std::string posX_fileName, std::string negX_fileName,
											   std::string posY_fileName, std::string negY_fileName,
											   std::string posZ_fileName, std::string negZ_fileName,
											   bool bIsSeamless, std::string& errorString);
			
			bool CreateCubeTextureFromPNGFiles(std::string cubeMapName,
											   std::string posX_fileName, std::string negX_fileName,
											   std::string posY_fileName, std::string negY_fileName,
											   std::string posZ_fileName, std::string negZ_fileName,
											   bool bIsSeamless, std::string& errorString);


			GLuint getTextureIDFromName(std::string textureFileName);

			void SetBasePath(std::string basepath);

			std::map<std::string, sTextureInfo*> textures;
			std::string base_path = "assets/textures/";

			bool error = false;
			GLenum errorEnum = 0;
			std::string errorDetails = "";
		};
	}
}