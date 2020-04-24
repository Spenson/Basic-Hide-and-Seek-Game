#include "Multi_Position.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		MultiPosition::MultiPosition() :iComponent(MULTI_POSITION_COMPONENT)
		{
		}
		bool MultiPosition::Deserialize(Json::Value& json)
		{
			if (!json["positions"].isArray())
			{
				return false;
			}
			glm::vec3 pos;

			Json::Value positions_json = json["positions"];
			for (unsigned int i = 0; i < positions_json.size(); i++)
			{
				JsonHelp::Set(positions_json[i], pos);

				positions.push_back(pos);
			}
			return true;
		}
	}
}
