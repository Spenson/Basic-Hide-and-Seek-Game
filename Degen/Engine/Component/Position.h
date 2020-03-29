#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int POSITION_COMPONENT = 3;

		class Position : public iComponent
		{
		public:
			Position();
			bool Deserialize(Json::Value& json) override;

			glm::vec3 position;
		};

	}
}
