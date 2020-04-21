#pragma once
#include <string>
#include <vector>
#include <map>

namespace Degen
{
	namespace Shaders
	{
		class cShaderManager
		{
		public:
			class cShader
			{
			public:
				cShader();
				~cShader();
				enum eShaderType
				{
					VERTEX_SHADER,
					TESSELATION_CONTROL_SHADER,
					TESSELATION_EVALUATION_SHADER,
					GEOMETRY_SHADER,
					FRAGMENT_SHADER,
					UNKNOWN
				};
				eShaderType shaderType;
				std::string getShaderTypeString(void);

				unsigned int ID;	// or "name" from OpenGL
				std::vector<std::string> vecSource;
				bool bSourceIsMultiLine;
				std::string fileName;

				//	map<string/*uniformName*/, int /*uniformlocation*/> mapUniformNameToLocationNumber;

			};

			class cShaderProgram
			{
			public:
				cShaderProgram() : ID(0) {};
				~cShaderProgram()  = default;
				unsigned int ID;			// ID from OpenGL (calls it a "name")
				std::string friendlyName;	// We give it this name

				// Look up the uniform location and save it.
				bool LoadUniformLocation(std::string variableName);

				// ** Call this once, when compiled and linked)
				// When called, this will load the map of uniforms. 
				void LoadActiveUniforms(void);
				
				// This looks up the map, instead of calling glGetUniformLocation() over and over
				int GetUniformLocationID(std::string uniformname);
				
				std::map<std::string /*uniform name*/,
					int /*uniform location*/> map_UniformNameToLocID;

			};

			cShaderManager();
			~cShaderManager();

			bool useShaderProgram(unsigned int ID);
			bool useShaderProgram(std::string friendlyName);

			// This will recreate the shader by default...
			bool createProgramFromFile(std::string friendlyName,
									   cShader& vertexShad,
									   cShader& fragShader,
									   bool replaceExisting = true);

			// This will recreate the shader by default...
			bool createProgramFromFile(std::string friendlyName,
									   cShader& vertexShad,
									   cShader& geomShad,
									   cShader& fragShader,
									   bool replaceExisting = true);

			// This will recreate the shader by default...
			bool createProgramFromFile(std::string friendlyName,
									   cShader& vertexShad,
									   cShader& tessCont,
									   cShader& tessEval,
									   cShader& geomShad,
									   cShader& fragShader,
									   bool replaceExisting = true);

			void setBasePath(std::string basepath);
			unsigned int GetProgramIDFromFriendlyName(std::string friendlyName);

			// Used to load the uniforms. Returns NULL if not found.
			cShaderProgram* pGetShaderProgramFromFriendlyName(std::string friendlyName);


			// Clears last error
			std::string getLastError(void);
		private:
			// Returns an empty string if it didn't work
			bool m_loadSourceFromFile(cShader& shader);
			std::string m_basepath;

			bool m_compileShaderFromSource(cShader& shader, std::string& error);
			// returns false if no error
			bool m_wasThereACompileError(unsigned int shaderID, std::string& errorText);
			bool m_wasThereALinkError(unsigned int programID, std::string& errorText);

			std::string m_lastError;

			std::map< unsigned int /*ID*/, cShaderProgram > m_ID_to_Shader;
			std::map< std::string /*name*/, unsigned int /*ID*/ > m_name_to_ID;
		};

	}
}


