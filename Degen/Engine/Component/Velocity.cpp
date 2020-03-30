#include "Velocity.h"
#include "../FileReading/JsonHelpers.h"


namespace Degen
{
	namespace Component
	{
		Velocity::Velocity() :iComponent(VELOCITY_COMPONENT),
			velocity(0.f)
		{
		}
		bool Velocity::Deserialize(Json::Value& json)
		{
			JsonHelp::Set(json["position"], velocity);

			return true;
		}
	}
}
