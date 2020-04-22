#include "BasicMotion.h"
#include "../FileReading/JsonHelpers.h"


namespace Degen
{
	namespace Component
	{
		BasicMotion::BasicMotion() :iComponent(BASIC_MOTION_COMPONENT),
			velocity(0.f)
		{
		}
		bool BasicMotion::Deserialize(Json::Value& json)
		{
			JsonHelp::Set(json["position"], velocity);

			return true;
		}
	}
}
