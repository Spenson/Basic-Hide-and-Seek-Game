#include "Motion.h"
#include "../FileReading/JsonHelpers.h"


namespace Degen
{
	namespace Component
	{
		Motion::Motion() : iComponent(MOTION_COMPONENT),
			max_velocity(1.f),
			velocity(0.f)
		{
		}

		bool Motion::Deserialize(Json::Value& json)
		{
			JsonHelp::Set(json["velovity"], velocity);
			JsonHelp::Set(json["max_vel"], max_velocity);

			return true;
		}
	}
}
