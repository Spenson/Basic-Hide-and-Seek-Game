#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int MOTION_COMPONENT = 8;

		class Motion : public iComponent
		{
		public:
			Motion();
			bool Deserialize(Json::Value& json) override;

			float max_velocity;
			
			glm::vec3 velocity;
		};

	}
}
