#pragma once
#include <string>
#include <glad\glad.h>
#include <vector>


namespace Degen
{
	namespace Texture
	{
		struct File
		{
			std::string name = "";
			unsigned height = 0;
			unsigned width = 0;
		};
		struct sTextureInfo
		{
			std::string name = "";

			bool is_2D_texture = false;
			File file;
			
			bool is_cubemap = false;
			std::vector<File> cube_files;
			
			GLuint texture_number = 0;

		};
	}
}
