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



		bool cTextureManager::Create2DTextureFromBMPFile(std::string textureFileName, bool bGenerateMIPMap)
		{


			return false;
		}

		bool cTextureManager::Create2DTextureFromPNGFile(std::string textureFileName, std::string friendly_name, bool bGenerateMIPMap)
		{
			sTextureInfo* info = new sTextureInfo();

			info->file_name = base_path + textureFileName;
			std::vector<unsigned char> image;
			unsigned width;
			unsigned height;

			lodepng::decode(image, info->width, info->height, info->file_name);

			glGenTextures(1, &(info->texture_number));
			glBindTexture(GL_TEXTURE_2D, info->texture_number);

			GLint GL_UNPACK_ALIGNMENT_old = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &GL_UNPACK_ALIGNMENT_old);
			// Set alignment to 1 byte
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glTexImage2D(GL_TEXTURE_2D,		// target (2D, 3D, etc.)		// OpenGL 2.0 and up
						 0,					// MIP map level 
						 GL_RGBA,			// internal format
						 info->width,	// width (pixels)	
						 info->height,		// height (pixels)	
						 0,					// border (0 or 1)
						 GL_RGBA,			// format of pixel data
						 GL_UNSIGNED_BYTE,	// type of pixel data
						 &image[0]);	// pointer to data in memory

			std::string errorstring;
			this->error = bWasThereAnOpenGLError(this->errorEnum, errorstring, this->errorDetails);
			if (this->error)
			{
				return false;
			}
			
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

		bool cTextureManager::CreateCubeTextureFromPNGFiles(std::string cubeMapName, std::string posX_fileName, std::string negX_fileName, std::string posY_fileName, std::string negY_fileName, std::string posZ_fileName, std::string negZ_fileName, bool bIsSeamless, std::string& errorString)
		{
			return false;
		}

		GLuint cTextureManager::getTextureIDFromName(std::string textureFileName)
		{
			std::map< std::string, sTextureInfo* >::iterator itTexture = this->textures.find(textureFileName);
			// Found it?
			if (itTexture == this->textures.end())
			{
				return 0;
			}
			// Reutrn texture number (from OpenGL genTexture)
			return itTexture->second->texture_number;
		}

		void cTextureManager::SetBasePath(std::string basepath)
		{
			this->base_path = basepath;
		}

	}
}

