#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int POSITION_TYPE = 2;

		class Position : public iComponent
		{
		public:
			Position();
			bool Serialize(Json::Value& json) override;

			glm::vec3 position;
		};

	}
}
