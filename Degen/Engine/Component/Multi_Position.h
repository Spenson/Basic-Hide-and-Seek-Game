#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int MULTI_POSITION_COMPONENT = 12;

		class MultiPosition : public iComponent
		{
		public:
			MultiPosition();
			bool Deserialize(Json::Value& json) override;

			std::vector<glm::vec3> positions;
		};
	}
}
