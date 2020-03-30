#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int VELOCITY_COMPONENT = 8;

		class Velocity : public iComponent
		{
		public:
			Velocity();
			bool Deserialize(Json::Value& json) override;

			glm::vec3 velocity;
		};

	}
}
