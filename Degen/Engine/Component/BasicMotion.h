#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int BASIC_MOTION_COMPONENT = 8;

		class BasicMotion : public iComponent
		{
		public:
			BasicMotion();
			bool Deserialize(Json::Value& json) override;

			glm::vec3 velocity;
			float radius;
		};

	}
}
