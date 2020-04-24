#include "Rotation.h"
#include "../FileReading/JsonHelpers.h"


namespace Degen
{
	namespace Component
	{
		Rotation::Rotation() :iComponent(ROTATION_COMPONENT),
			rotation(glm::vec3(0.f))
		{
		}
		bool Rotation::Deserialize(Json::Value& json)
		{
			glm::vec3 rot;
			JsonHelp::Set(json["rotation"], rot);

			rotation = glm::quat(glm::radians(rot));

			return true;
		}
	}
}
