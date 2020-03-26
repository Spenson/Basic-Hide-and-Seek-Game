#include "Position.h"
#include "../FileReading/JsonHelpers.h"


namespace Degen
{
	namespace Component
	{
		Position::Position() :iComponent(POSITION_TYPE),
			position(0.f)
		{
		}
		bool Position::Serialize(Json::Value& json)
		{
			JsonHelp::Set(json["position"], position);
			
			return true;
		}
	}
}
