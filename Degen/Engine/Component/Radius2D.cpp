#include "Radius2D.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		Radius2D::Radius2D() : iComponent(RADIUS_2D_COMPONENT), radius(1.f)
		{
		}
		bool Radius2D::Deserialize(Json::Value& json)
		{
			JsonHelp::Set(json["radius"], radius);
			
			return true;
		}
	}
}
