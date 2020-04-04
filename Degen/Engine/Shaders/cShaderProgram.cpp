#include "cShaderManager.h"
#include <glad/glad.h>

#ifdef _DEBUG
#include <iostream>
#endif

namespace Degen
{
	namespace Shaders
	{
		bool cShaderManager::cShaderProgram::LoadUniformLocation(std::string variableName)
		{
			GLint uniLocation = glGetUniformLocation(this->ID, variableName.c_str());
			// Did it find it (not -1)
			if (uniLocation == -1)
			{	// Nope.
				return false;
			}
			// Save it
			this->map_UniformNameToLocID[variableName] = uniLocation;

			return true;
		}
		void cShaderManager::cShaderProgram::LoadActiveUniforms(void)
		{
			glUseProgram(this->ID);
			// Find out the max size of the buffer we need. 
			int maxNameSize = 0;
			glGetProgramiv(this->ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameSize);

#ifdef _DEBUG
			std::cout << "Max uniform name size is : " << maxNameSize << std::endl;
#endif

			// Create a buffer of this max size
			char* uniformName = new char[maxNameSize];
			// Clear it
			memset(uniformName, 0, maxNameSize);

			// How many uniforms? 
			int numberOfActiveUniforms = 0;
			glGetProgramiv(this->ID, GL_ACTIVE_UNIFORMS, &numberOfActiveUniforms);

#ifdef _DEBUG
			std::cout << "There are " << numberOfActiveUniforms << " active uniforms" << std::endl;
#endif

			int uniformlocation = 0;

			for (int index = 0; index < numberOfActiveUniforms; index++)
			{
				const int BUFFERSIZE = 1000;
				char myBuffer[BUFFERSIZE] = { 0 };

				GLsizei numCharWritten = 0;
				GLint sizeOfVariable = 0;
				GLenum uniformType = 0;
				memset(uniformName, 0, maxNameSize);

				glGetActiveUniform(this->ID,
								   index,				// NOT UNIFORM LOCATION!!!!! Uniform index (0 to ...)
								   BUFFERSIZE,			// Max number of chars
								   &numCharWritten,		// How many it REALLY wrote
								   &sizeOfVariable,		// Size in bytes?
								   &uniformType,
								   uniformName);

				uniformlocation = glGetUniformLocation(this->ID, uniformName);

#ifdef _DEBUG
				std::cout << "Uniform # " << index << "\t" << uniformlocation << "\t" << uniformName << "\t" << sizeOfVariable << std::endl;
#endif

				this->map_UniformNameToLocID[uniformName] = uniformlocation;



			}//for (int index = 0

			return;
		}
		int cShaderManager::cShaderProgram::GetUniformLocationID(std::string uniformname)
		{
			std::map<std::string, GLint>::iterator itUniform = map_UniformNameToLocID.find(uniformname);

			if (itUniform != map_UniformNameToLocID.end())
			{
				return itUniform->second;
			}

			// Didn't find it.
			printf("No Uniform Location for %s\n", uniformname.c_str());
			return -1;
		}
	}
}
