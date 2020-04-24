#include "AABB2D.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		AABB2D::AABB2D() :iComponent(AABB_2D_COMPONENT),
			half_extents(0.f)
		{
		}
		bool AABB2D::Deserialize(Json::Value& json)
		{
			JsonHelp::Set(json["half_extents"], half_extents);

			return true;
		}
	}
}
