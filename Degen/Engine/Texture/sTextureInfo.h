#pragma once
#include <string>
#include <glad\glad.h>


namespace Degen
{
	namespace Texture
	{
		struct sTextureInfo
		{
			std::string name = "";
			std::string file_name = "";

			bool is_cubemap = false;
			bool is_2D_texture = false;
			unsigned height = 0;
			unsigned width = 0;

			GLuint texture_number = 0;

		};
	}
}