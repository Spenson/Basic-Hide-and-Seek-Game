#include "cTextureManager.h"
#include "lodepng.h"

namespace Degen
{
	namespace Texture
	{
		bool bWasThereAnOpenGLError(GLenum& errorEnum, std::string& errorString, std::string& errorDetails)
		{
			errorEnum = glGetError();
			// from: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetError.xml
			switch (errorEnum)
			{
				case GL_NO_ERROR:
					errorString = "GL_NO_ERROR";
					errorDetails = "GL_NO_ERROR: No error has been recorded.";
					/******************/
					return false;
					/******************/
					break;
				case GL_INVALID_ENUM:
					errorString = "GL_INVALID_ENUM";
					errorDetails = "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
					break;
				case GL_INVALID_VALUE:
					errorString = "GL_INVALID_VALUE";
					errorDetails = "GL_INVALID_VALUE: A numeric argument is out of range.";
					break;
				case GL_INVALID_OPERATION:
					errorString = "GL_INVALID_OPERATION";
					errorDetails = "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
					errorDetails = "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
					break;
				case GL_OUT_OF_MEMORY:
					errorString = "GL_OUT_OF_MEMORY";
					errorDetails = "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
					break;
				case GL_STACK_UNDERFLOW:
					errorString = "GL_STACK_UNDERFLOW";
					errorDetails = "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
					break;
				case GL_STACK_OVERFLOW:
					errorString = "GL_STACK_OVERFLOW";
					errorDetails = "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
					break;
				default:
					errorString = "Unknown error";
					errorDetails = "Unknown error";
					break;
			};

			return true;	// There -WAS- an error
		}



		bool cTextureManager::Create2DTextureFromBMPFile(std::string textureFileName, std::string friendly_name, std::string& error, bool bGenerateMIPMap)
		{


			return false;
		}

		bool cTextureManager::Create2DTextureFromPNGFile(std::string textureFileName, std::string friendly_name, std::string& error, bool bGenerateMIPMap)
		{
			error.clear();
			sTextureInfo* info = new sTextureInfo();

			info->file.name = base_path + textureFileName;
			std::vector<unsigned char> image;

			if (lodepng::decode(image, info->file.width, info->file.height, info->file.name))
			{
				error = "Unable to decode file: " + info->file.name;
				return false;
			}

			glGenTextures(1, &(info->texture_number));
			glBindTexture(GL_TEXTURE_2D, info->texture_number);

			GLint GL_UNPACK_ALIGNMENT_old = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &GL_UNPACK_ALIGNMENT_old);
			// Set alignment to 1 byte
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glTexImage2D(GL_TEXTURE_2D,		// target (2D, 3D, etc.)		// OpenGL 2.0 and up
						 0,					// MIP map level 
						 GL_RGBA,			// internal format
						 info->file.width,	// width (pixels)	
						 info->file.height,		// height (pixels)	
						 0,					// border (0 or 1)
						 GL_RGBA,			// format of pixel data
						 GL_UNSIGNED_BYTE,	// type of pixel data
						 &image[0]);	// pointer to data in memory

			std::string errorstring;
			GLenum errorEnum;
			if (bWasThereAnOpenGLError(errorEnum, errorstring, error))
			{
				return false;
			}
			error.clear();

			if (bGenerateMIPMap)
			{
				glGenerateMipmap(GL_TEXTURE_2D);		// OpenGL 4.0
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_CLAMP*/ GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, /*GL_CLAMP*/ GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST /*GL_LINEAR*/);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


			info->is_2D_texture = true;

			textures[friendly_name] = info;

			return true;
		}

		bool cTextureManager::CreateCubeTextureFromPNGFiles(std::string cubeMapName, std::string posX_fileName, std::string negX_fileName, std::string posY_fileName, std::string negY_fileName, std::string posZ_fileName, std::string negZ_fileName, bool bIsSeamless, std::string& error)
		{
			std::string errorstring;
			GLenum errorEnum;

			sTextureInfo* info = new sTextureInfo();
			std::vector<unsigned char> posX_image;

			info->cube_files.push_back(File());
			info->cube_files.back().name = base_path + posX_fileName;
			if (lodepng::decode(posX_image, info->cube_files.back().width, info->cube_files.back().height, info->cube_files.back().name))
			{
				error = "Unable to decode file: " + info->cube_files.back().name;
				return false;
			}

			std::vector<unsigned char> negX_image;
			info->cube_files.push_back(File());
			info->cube_files.back().name = base_path + negX_fileName;
			if (lodepng::decode(negX_image, info->cube_files.back().width, info->cube_files.back().height, info->cube_files.back().name))
			{
				error = "Unable to decode file: " + info->cube_files.back().name;
				return false;
			}

			std::vector<unsigned char> posY_image;
			info->cube_files.push_back(File());
			info->cube_files.back().name = base_path + posY_fileName;
			if (lodepng::decode(posY_image, info->cube_files.back().width, info->cube_files.back().height, info->cube_files.back().name))
			{
				error = "Unable to decode file: " + info->cube_files.back().name;
				return false;
			}

			std::vector<unsigned char> negY_image;
			info->cube_files.push_back(File());
			info->cube_files.back().name = base_path + negY_fileName;
			if (lodepng::decode(negY_image, info->cube_files.back().width, info->cube_files.back().height, info->cube_files.back().name))
			{
				error = "Unable to decode file: " + info->cube_files.back().name;
				return false;
			}

			std::vector<unsigned char> posZ_image;
			info->cube_files.push_back(File());
			info->cube_files.back().name = base_path + posZ_fileName;
			if (lodepng::decode(posZ_image, info->cube_files.back().width, info->cube_files.back().height, info->cube_files.back().name))
			{
				error = "Unable to decode file: " + info->cube_files.back().name;
				return false;
			}

			std::vector<unsigned char> negZ_image;
			info->cube_files.push_back(File());
			info->cube_files.back().name = base_path + negZ_fileName;
			if (lodepng::decode(negZ_image, info->cube_files.back().width, info->cube_files.back().height, info->cube_files.back().name))
			{
				error = "Unable to decode file: " + info->cube_files.back().name;
				return false;
			}


			glGenTextures(1, &(info->texture_number));
			// Worked?
			if ((glGetError() & GL_INVALID_VALUE) == GL_INVALID_VALUE)
			{
				error = "Failed to generate texture1.";
				return false;
			}

			glBindTexture(GL_TEXTURE_CUBE_MAP, info->texture_number);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			if (bIsSeamless)
			{
				glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);	// OpenGL 4.1, maybe
			}


			if (bWasThereAnOpenGLError(errorEnum, errorstring, error)) { return false; }
			error.clear();


			glTexStorage2D(GL_TEXTURE_CUBE_MAP,
						   10,							// Mipmap levels
						   GL_RGBA8,					// Internal format
						   info->cube_files[0].width,	// width (pixels)
						   info->cube_files[0].height);	// height (pixels)

			if (bWasThereAnOpenGLError(errorEnum, errorstring, error)) { return false; }
			error.clear();


			{

				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
								0,   // Level
								0, 0, // Offset
								info->cube_files[0].width,	// width
								info->cube_files[0].height,	// height
								GL_RGBA,
								GL_UNSIGNED_BYTE,
								&posX_image[0]);

				if (bWasThereAnOpenGLError(errorEnum, errorstring, error)) { return false; }
				error.clear();

			}
			{

				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
								0,   // Level
								0, 0, // Offset
								info->cube_files[1].width,	// width
								info->cube_files[1].height,	// height
								GL_RGBA,
								GL_UNSIGNED_BYTE,
								&negX_image[0]);

				if (bWasThereAnOpenGLError(errorEnum, errorstring, error)) { return false; }
				error.clear();

			}


			{

				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
								0,   // Level
								0, 0, // Offset
								info->cube_files[2].width,	// width
								info->cube_files[2].height,	// height
								GL_RGBA,
								GL_UNSIGNED_BYTE,
								&posY_image[0]);

				if (bWasThereAnOpenGLError(errorEnum, errorstring, error)) { return false; }
				error.clear();

			}
			{

				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
								0,   // Level
								0, 0, // Offset
								info->cube_files[3].width,	// width
								info->cube_files[3].height,	// height
								GL_RGBA,
								GL_UNSIGNED_BYTE,
								&negY_image[0]);

				if (bWasThereAnOpenGLError(errorEnum, errorstring, error)) { return false; }
				error.clear();

			}


			{

				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
								0,   // Level
								0, 0, // Offset
								info->cube_files[4].width,	// width
								info->cube_files[4].height,	// height
								GL_RGBA,
								GL_UNSIGNED_BYTE,
								&posZ_image[0]);

				if (bWasThereAnOpenGLError(errorEnum, errorstring, error)) { return false; }
				error.clear();

			}
			{

				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
								0,   // Level
								0, 0, // Offset
								info->cube_files[5].width,	// width
								info->cube_files[5].height,	// height
								GL_RGBA,
								GL_UNSIGNED_BYTE,
								&negZ_image[0]);

				if (bWasThereAnOpenGLError(errorEnum, errorstring, error)) { return false; }
				error.clear();

			}

			info->name = cubeMapName;
			info->is_cubemap = true;

			textures[cubeMapName] = info;
			
			return true;
		}

		GLuint cTextureManager::getTextureIDFromName(std::string textureFileName)
		{
			std::map< std::string, sTextureInfo* >::iterator itTexture = this->textures.find(textureFileName);
			// Found it?
			if (itTexture == this->textures.end())
			{
				printf("Texture %s not found.\n", textureFileName.c_str());
				return 0;
			}
			// Reutrn texture1 number (from OpenGL genTexture)
			return itTexture->second->texture_number;
		}

		void cTextureManager::SetBasePath(const std::string& basepath)
		{
			this->base_path = basepath;
		}

	}
}

