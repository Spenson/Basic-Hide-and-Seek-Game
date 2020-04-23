#include "MultiTransform.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		MultiTransform::MultiTransform() : iComponent(MULTI_TRANSFORM_COMPONENT)
		{
		}
		bool MultiTransform::Deserialize(Json::Value& json)
		{
			if (!json["transforms"].isArray())
			{
				return false;
			}
			glm::vec3 pos;
			glm::vec3 rot;
			glm::mat4 transform = glm::mat4(1.f);


			Json::Value transforms_json = json["transforms"];
			for (unsigned int i = 0; i < transforms_json.size(); i++)
			{
				JsonHelp::Set(transforms_json[i]["pos"], pos);
				JsonHelp::Set(transforms_json[i]["rot"], rot);
				
				transform = glm::mat4(1.f);
				transform = glm::translate(transform, pos);
				transform *= glm::mat4(glm::quat(glm::radians(rot)));

				transforms.push_back(transform);
			}

		
			return true;
		}
	}
}
