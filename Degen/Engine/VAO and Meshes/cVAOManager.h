#pragma once
#include <string>
#include <map>

#include "sModelDrawInfo.h"

namespace Degen
{
	namespace VAOAndModel
	{
		class cVAOManager
		{
		public:

			bool LoadModelDrawInfoIntoVAO(sModelDrawInfo& drawInfo,
										  unsigned int shaderProgramID);

			bool FindDrawInfoByModelName(std::string filename,
										 sModelDrawInfo& drawInfo);
			
		private:

			std::map< std::string /*model name*/,
				Degen::VAOAndModel::sModelDrawInfo /* info needed to draw*/ >
				m_name_draw_info;

		};

	}
}