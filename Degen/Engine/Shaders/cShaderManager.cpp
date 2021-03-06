#include "cShaderManager.h"
#include <glad/glad.h>
#include <sstream>
#include <fstream>

namespace Degen
{
	namespace Shaders
	{
		const unsigned int MAXLINELENGTH = 65536;		// 16x1024

		cShaderManager::cShaderManager()
		{
			return;
		}
		cShaderManager::~cShaderManager()
		{
			return;
		}
		bool cShaderManager::useShaderProgram(unsigned int ID)
		{
			// Use the number directy...
			if (m_ID_to_Shader.find(ID) == m_ID_to_Shader.end())
			{
				return false;
			}
			glUseProgram(ID);
			return true;
		}
		bool cShaderManager::useShaderProgram(std::string friendlyName)
		{
			std::map< std::string, unsigned int >::iterator itShad = this->m_name_to_ID.find(friendlyName);

			if (itShad == this->m_name_to_ID.end())
			{
				// Didn't find it
				return false;
			}
			glUseProgram(itShad->second);

			return true;
		}
		bool cShaderManager::createProgramFromFile(std::string friendlyName, cShader& vertexShad, cShader& fragShader, bool replaceExisting)
		{
			std::string errorText = "";

			// See if the shader is already there (the name)
			cShaderManager::cShaderProgram* pShaderProgram = this->pGetShaderProgramFromFriendlyName(friendlyName);

			if (pShaderProgram != NULL)
			{
				// There is an existing shader, already

				if (replaceExisting)
				{
					// TODO: Find the shader in the two maps and delete them from the map
					// (If it's single threaded, do I ever need to do this???)

					// Call glDeleteShader() with the ID, to delete from the GPU
					glDeleteShader(pShaderProgram->ID);

					// TODO: Delete the shader pointer
					delete pShaderProgram;

					pShaderProgram = NULL;
				}
				else
				{
					return false;
				}

			}//if (pShaderProgram != NULL)


			// At this point, there ISN'T a shader (never was or was deleted)



			// Shader loading happening before vertex buffer array
			vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
			vertexShad.shaderType = cShader::VERTEX_SHADER;
			//  char* vertex_shader_text = "wewherlkherlkh";
			// Load some text from a file...
			if (!this->m_loadSourceFromFile(vertexShad))
			{
				return false;
			}//if ( ! this->m_loadSourceFromFile(...

			//errorText = "";
			if (!this->m_compileShaderFromSource(vertexShad, errorText))
			{
				this->m_lastError = errorText;
				return false;
			}//if ( this->m_compileShaderFromSource(...



			//*****
			fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
			fragShader.shaderType = cShader::FRAGMENT_SHADER;
			if (!this->m_loadSourceFromFile(fragShader))
			{
				return false;
			}//if ( ! this->m_loadSourceFromFile(...

			if (!this->m_compileShaderFromSource(fragShader, errorText))
			{
				this->m_lastError = errorText;
				return false;
			}//if ( this->m_compileShaderFromSource(...


			cShaderProgram curProgram;
			curProgram.ID = glCreateProgram();

			glAttachShader(curProgram.ID, vertexShad.ID);
			glAttachShader(curProgram.ID, fragShader.ID);
			glLinkProgram(curProgram.ID);

			// Was there a link error? 
			//errorText = "";
			if (this->m_wasThereALinkError(curProgram.ID, errorText))
			{
				std::stringstream ssError;
				ssError << "Shader program link error: ";
				ssError << errorText;
				this->m_lastError = ssError.str();
				return false;
			}

			// At this point, shaders are compiled and linked into a program

			curProgram.friendlyName = friendlyName;

			// Add the shader to the map
			this->m_ID_to_Shader[curProgram.ID] = curProgram;
			// Save to other map, too
			this->m_name_to_ID[curProgram.friendlyName] = curProgram.ID;

			return true;
		}
		bool cShaderManager::createProgramFromFile(std::string friendlyName, cShader& vertexShad, cShader& geomShad, cShader& fragShader, bool replaceExisting)
		{
			std::string errorText = "";

			// See if the shader is already there (the name)
			cShaderManager::cShaderProgram* pShaderProgram = this->pGetShaderProgramFromFriendlyName(friendlyName);

			if (pShaderProgram != NULL)
			{
				// There is an existing shader, already

				if (replaceExisting)
				{
					// TODO: Find the shader in the two maps and delete them from the map
					// (If it's single threaded, do I ever need to do this???)

					// Call glDeleteShader() with the ID, to delete from the GPU
					glDeleteShader(pShaderProgram->ID);

					// TODO: Delete the shader pointer
					delete pShaderProgram;

					pShaderProgram = NULL;
				}
				else
				{
					return false;
				}

			}//if (pShaderProgram != NULL)


			// At this point, there ISN'T a shader (never was or was deleted)

			// Shader loading happening before vertex buffer array
			vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
			vertexShad.shaderType = cShader::VERTEX_SHADER;
			//  char* vertex_shader_text = "wewherlkherlkh";
			// Load some text from a file...
			if (!this->m_loadSourceFromFile(vertexShad))
			{
				return false;
			}//if ( ! this->m_loadSourceFromFile(...

			//errorText = "";
			if (!this->m_compileShaderFromSource(vertexShad, errorText))
			{
				this->m_lastError = errorText;
				return false;
			}//if ( this->m_compileShaderFromSource(...






			// ******
			// Shader loading happening before vertex buffer array
			geomShad.ID = glCreateShader(GL_GEOMETRY_SHADER);
			geomShad.shaderType = cShader::GEOMETRY_SHADER;
			//  char* vertex_shader_text = "wewherlkherlkh";
			// Load some text from a file...
			if (!this->m_loadSourceFromFile(geomShad))
			{
				return false;
			}//if ( ! this->m_loadSourceFromFile(...

			//errorText = "";
			if (!this->m_compileShaderFromSource(geomShad, errorText))
			{
				this->m_lastError = errorText;
				return false;
			}//if ( this->m_compileShaderFromSource(...








			// ******
			fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
			fragShader.shaderType = cShader::FRAGMENT_SHADER;
			if (!this->m_loadSourceFromFile(fragShader))
			{
				return false;
			}//if ( ! this->m_loadSourceFromFile(...

			if (!this->m_compileShaderFromSource(fragShader, errorText))
			{
				this->m_lastError = errorText;
				return false;
			}//if ( this->m_compileShaderFromSource(...


			cShaderProgram curProgram;
			curProgram.ID = glCreateProgram();

			glAttachShader(curProgram.ID, vertexShad.ID);
			//**********************
			glAttachShader(curProgram.ID, geomShad.ID);
			//**********************
			glAttachShader(curProgram.ID, fragShader.ID);
			glLinkProgram(curProgram.ID);

			// Was there a link error? 
			//errorText = "";
			if (this->m_wasThereALinkError(curProgram.ID, errorText))
			{
				std::stringstream ssError;
				ssError << "Shader program link error: ";
				ssError << errorText;
				this->m_lastError = ssError.str();
				return false;
			}

			// At this point, shaders are compiled and linked into a program

			curProgram.friendlyName = friendlyName;

			// Add the shader to the map
			this->m_ID_to_Shader[curProgram.ID] = curProgram;
			// Save to other map, too
			this->m_name_to_ID[curProgram.friendlyName] = curProgram.ID;

			return true;
		}





		
		bool cShaderManager::createProgramFromFile(std::string friendlyName, cShader& vertexShad, cShader& tessCont, cShader& tessEval, cShader& geomShad, cShader& fragShader, bool replaceExisting)
		{
			std::string errorText = "";

			// See if the shader is already there (the name)
			cShaderManager::cShaderProgram* pShaderProgram = this->pGetShaderProgramFromFriendlyName(friendlyName);

			if (pShaderProgram != NULL)
			{
				// There is an existing shader, already

				if (replaceExisting)
				{
					// TODO: Find the shader in the two maps and delete them from the map
					// (If it's single threaded, do I ever need to do this???)

					// Call glDeleteShader() with the ID, to delete from the GPU
					glDeleteShader(pShaderProgram->ID);

					// TODO: Delete the shader pointer
					delete pShaderProgram;

					pShaderProgram = NULL;
				}
				else
				{
					return false;
				}

			}//if (pShaderProgram != NULL)


			// At this point, there ISN'T a shader (never was or was deleted)

			// Shader loading happening before vertex buffer array
			vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
			vertexShad.shaderType = cShader::VERTEX_SHADER;
			//  char* vertex_shader_text = "wewherlkherlkh";
			// Load some text from a file...
			if (!this->m_loadSourceFromFile(vertexShad))
			{
				return false;
			}//if ( ! this->m_loadSourceFromFile(...

			//errorText = "";
			if (!this->m_compileShaderFromSource(vertexShad, errorText))
			{
				this->m_lastError = errorText;
				return false;
			}//if ( this->m_compileShaderFromSource(...





			
			// ******


			
			// Shader loading happening before vertex buffer array
			tessCont.ID = glCreateShader(GL_TESS_CONTROL_SHADER);
			tessCont.shaderType = cShader::TESSELATION_CONTROL_SHADER;
			//  char* vertex_shader_text = "wewherlkherlkh";
			// Load some text from a file...
			if (!this->m_loadSourceFromFile(tessCont))
			{
				return false;
			}//if ( ! this->m_loadSourceFromFile(...

			//errorText = "";
			if (!this->m_compileShaderFromSource(tessCont, errorText))
			{
				this->m_lastError = errorText;
				return false;
			}//if ( this->m_compileShaderFromSource(...

			
			// Shader loading happening before vertex buffer array
			tessEval.ID = glCreateShader(GL_TESS_EVALUATION_SHADER);
			tessEval.shaderType = cShader::TESSELATION_EVALUATION_SHADER;
			//  char* vertex_shader_text = "wewherlkherlkh";
			// Load some text from a file...
			if (!this->m_loadSourceFromFile(tessEval))
			{
				return false;
			}//if ( ! this->m_loadSourceFromFile(...

			//errorText = "";
			if (!this->m_compileShaderFromSource(tessEval, errorText))
			{
				this->m_lastError = errorText;
				return false;
			}//if ( this->m_compileShaderFromSource(...








			




			// ******
			// Shader loading happening before vertex buffer array
			geomShad.ID = glCreateShader(GL_GEOMETRY_SHADER);
			geomShad.shaderType = cShader::GEOMETRY_SHADER;
			//  char* vertex_shader_text = "wewherlkherlkh";
			// Load some text from a file...
			if (!this->m_loadSourceFromFile(geomShad))
			{
				return false;
			}//if ( ! this->m_loadSourceFromFile(...

			//errorText = "";
			if (!this->m_compileShaderFromSource(geomShad, errorText))
			{
				this->m_lastError = errorText;
				return false;
			}//if ( this->m_compileShaderFromSource(...

			// ******
			fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
			fragShader.shaderType = cShader::FRAGMENT_SHADER;
			if (!this->m_loadSourceFromFile(fragShader))
			{
				return false;
			}//if ( ! this->m_loadSourceFromFile(...

			if (!this->m_compileShaderFromSource(fragShader, errorText))
			{
				this->m_lastError = errorText;
				return false;
			}//if ( this->m_compileShaderFromSource(...


			cShaderProgram curProgram;
			curProgram.ID = glCreateProgram();

			glAttachShader(curProgram.ID, vertexShad.ID);
			//**********************
			glAttachShader(curProgram.ID, tessCont.ID);
			glAttachShader(curProgram.ID, tessEval.ID);
			//**********************
			glAttachShader(curProgram.ID, geomShad.ID);
			glAttachShader(curProgram.ID, fragShader.ID);
			glLinkProgram(curProgram.ID);

			// Was there a link error? 
			//errorText = "";
			if (this->m_wasThereALinkError(curProgram.ID, errorText))
			{
				std::stringstream ssError;
				ssError << "Shader program link error: ";
				ssError << errorText;
				this->m_lastError = ssError.str();
				return false;
			}

			// At this point, shaders are compiled and linked into a program

			curProgram.friendlyName = friendlyName;

			// Add the shader to the map
			this->m_ID_to_Shader[curProgram.ID] = curProgram;
			// Save to other map, too
			this->m_name_to_ID[curProgram.friendlyName] = curProgram.ID;

			return true;
		}






		
		void cShaderManager::setBasePath(std::string basepath)
		{
			this->m_basepath = basepath;
			return;
		}
		unsigned int cShaderManager::GetProgramIDFromFriendlyName(std::string friendlyName)
		{
			std::map< std::string, unsigned int>::iterator itShad = this->m_name_to_ID.find(friendlyName);

			if (itShad == this->m_name_to_ID.end())
			{	// Didn't find it
				return 0;
			}
			return itShad->second;
		}
		cShaderManager::cShaderProgram* cShaderManager::pGetShaderProgramFromFriendlyName(std::string friendlyName)
		{
			unsigned int shaderID = this->GetProgramIDFromFriendlyName(friendlyName);

			// Now get the actual shader

			std::map< unsigned int, cShaderProgram >::iterator itShad = this->m_ID_to_Shader.find(shaderID);

			if (itShad == this->m_ID_to_Shader.end())
			{
				// Didn't find it
				return NULL;		// or 0 or nullptr
			}

			cShaderProgram* pShaderIFound = &(itShad->second);

			return pShaderIFound;
		}
		std::string cShaderManager::getLastError(void)
		{
			std::string lastErrorTemp = this->m_lastError;
			this->m_lastError = "";
			return lastErrorTemp;
		}
		bool cShaderManager::m_loadSourceFromFile(cShader& shader)
		{
			std::string fullFileName = this->m_basepath + shader.fileName;

			std::ifstream theFile(fullFileName.c_str());
			if (!theFile.is_open())
			{
				return false;
			}

			//std::stringstream ssSource;
			//std::string temp;

			//while ( theFile >> temp )
			//{	// Add it to the string stream
			//	ssSource << temp << " ";
			//}

			shader.vecSource.clear();

			char pLineTemp[MAXLINELENGTH] = { 0 };
			while (theFile.getline(pLineTemp, MAXLINELENGTH))
			{
				std::string tempString(pLineTemp);
				//if ( tempString != "" )
				//{	// Line isn't empty, so add
				shader.vecSource.push_back(tempString);
				//}
			}

			theFile.close();
			return true;		// Return the string (from the sstream)
		}
		bool cShaderManager::m_compileShaderFromSource(cShader& shader, std::string& error)
		{
			error = "";

			const unsigned int MAXLINESIZE = 8 * 1024;	// About 8K PER LINE, which seems excessive

			unsigned int numberOfLines = static_cast<unsigned int>(shader.vecSource.size());

			// This is an array of pointers to strings. aka the lines of source
			char** arraySource = new char* [numberOfLines];
			// Clear array to all zeros (i.e. '\0' or null terminator)
			memset(arraySource, 0, numberOfLines);

			for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
			{
				unsigned int numCharacters = (unsigned int)shader.vecSource[indexLine].length();
				// Create an array of chars for each line
				arraySource[indexLine] = new char[numCharacters + 2];		// For the '\n' and '\0' at end
				memset(arraySource[indexLine], 0, numCharacters + 2);

				// Copy line of source into array
				for (unsigned int indexChar = 0; indexChar != shader.vecSource[indexLine].length(); indexChar++)
				{
					arraySource[indexLine][indexChar] = shader.vecSource[indexLine][indexChar];
				}//for ( unsigned int indexChar = 0...

				// At a '\0' at end (just in case)
				arraySource[indexLine][numCharacters + 0] = '\n';
				arraySource[indexLine][numCharacters + 1] = '\0';

				// Or you could use the (unsecure) strcpy (string copy)
		//		strcpy( arraySource[indexLine], shader.vecSource[indexLine].c_str() );

				// Or the "secure" version, where you specify the number of chars to copy
		//		strcpy_s( arraySource[indexLine], 
		//				  strlen( shader.vecSource[indexLine].c_str() + 1 ),	// Number of char in copy + 0 terminator
		//				  shader.vecSource[indexLine].c_str() );

			}//for ( unsigned int indexLine = 0...

		//******************************************************************************************
		//	// Print it out (for debug)
		//	std::cout << "Source (from char** array):" << std::endl;
		//	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
		//	{
		//		std::cout << "(" << indexLine << "):\t" << arraySource[indexLine] << std::endl;
		//	}
		//	std::cout << "END" << std::endl;
		//******************************************************************************************

			//const char* tempVertChar = "Hey there;";

			glShaderSource(shader.ID, numberOfLines, arraySource, NULL);
			glCompileShader(shader.ID);

			// Get rid of the temp source "c" style array
			for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
			{	// Delete this line
				delete[] arraySource[indexLine];
			}
			// And delete the original char** array
			delete[] arraySource;

			// Did it work? 
			std::string errorText = "";
			if (this->m_wasThereACompileError(shader.ID, errorText))
			{
				std::stringstream ssError;
				ssError << shader.getShaderTypeString();
				ssError << " compile error: ";
				ssError << errorText;
				error = ssError.str();
				return false;
			}

			return true;
		}
		bool cShaderManager::m_wasThereACompileError(unsigned int shaderID, std::string& errorText)
		{
			errorText = "";	// No error

			GLint isCompiled = 0;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

				char* pLogText = new char[maxLength];
				// Fill with zeros, maybe...?
				glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);
				// Copy char array to string
				errorText.append(pLogText);

				// Extra code that Michael forgot wasn't there... 
				this->m_lastError.append("\n");
				this->m_lastError.append(errorText);


				delete[] pLogText;	// Oops

				return true;	// There WAS an error
			}
			return false; // There WASN'T an error
		}
		bool cShaderManager::m_wasThereALinkError(unsigned int programID, std::string& errorText)
		{
			errorText = "";	// No error

			GLint wasError = 0;
			glGetProgramiv(programID, GL_LINK_STATUS, &wasError);
			if (wasError == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

				char* pLogText = new char[maxLength];
				// Fill with zeros, maybe...?
				glGetProgramInfoLog(programID, maxLength, &maxLength, pLogText);
				// Copy char array to string
				errorText.append(pLogText);

				// Extra code that Michael forgot wasn't there... 
				this->m_lastError.append("\n");
				this->m_lastError.append(errorText);


				delete[] pLogText;

				// There WAS an error
				return true;
			}

			// There WASN'T an error
			return false;
		}
	}
}
