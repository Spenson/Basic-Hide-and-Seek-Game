#include "cShaderManager.h"

namespace Degen
{
	namespace Shaders
	{
		cShaderManager::cShader::cShader()
		{
			this->ID = 0;
			this->shaderType = cShader::UNKNOWN;
			return;
		}
		cShaderManager::cShader::~cShader()
		{
			return;
		}
		std::string cShaderManager::cShader::getShaderTypeString(void)
		{
			switch (this->shaderType)
			{
				case cShader::VERTEX_SHADER:
					return "VERTEX_SHADER";
					break;
				case cShader::TESSELATION_CONTROL_SHADER:
					return "TESSELATION_CONTROL_SHADER";
					break;
				case cShader::TESSELATION_EVALUATION_SHADER:
					return "TESSELATION_EVALUATION_SHADER";
					break;
				case cShader::GEOMETRY_SHADER:
					return "GEOMETRY_SHADER";
					break;
				case cShader::FRAGMENT_SHADER:
					return "FRAGMENT_SHADER";
					break;
				case cShader::UNKNOWN:
				default:
					return "UNKNOWN_SHADER_TYPE";
					break;
			}
			// Should never reach here...
			return "UNKNOWN_SHADER_TYPE";
		}
	}
}