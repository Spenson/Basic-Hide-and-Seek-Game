#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int AABB_2D_COMPONENT = 14;

		class AABB2D : public iComponent
		{
		public:
			AABB2D();
			bool Deserialize(Json::Value& json) override;

			glm::vec2 half_extents;
		};
	}
}
