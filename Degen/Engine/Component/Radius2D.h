#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int RADIUS_2D_COMPONENT = 13;

		class Radius2D : public iComponent
		{
		public:
			Radius2D();
			bool Deserialize(Json::Value& json) override;

			float radius;
		};
	}
}
