#include "Rotation.h"
#include "../FileReading/JsonHelpers.h"


namespace Degen
{
	namespace Component
	{
		Rotation::Rotation() :iComponent(ROTATION_TYPE),
			rotation(glm::vec3(0.f))
		{
		}
		bool Rotation::Serialize(Json::Value& json)
		{
			glm::vec3 rot;
			JsonHelp::Set(json["rotation"], rot);

			rotation = glm::quat(rot);

			return true;
		}
	}
}
