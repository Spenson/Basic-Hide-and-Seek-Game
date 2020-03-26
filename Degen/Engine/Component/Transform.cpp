#include "Transform.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		Transform::Transform() :iComponent(TRANSFORM_TYPE),
			transform(1.f)
		{

		}
		bool Transform::Serialize(Json::Value& json)
		{
			glm::vec3 pos;
			glm::vec3 rot;
			
			JsonHelp::Set(json["pos"], pos);
			JsonHelp::Set(json["rot"], rot);

			transform = glm::mat4(1.f);
			transform = glm::translate(transform, pos);
			transform *= glm::quat(rot);
			
			return true;
		}
	}
}
