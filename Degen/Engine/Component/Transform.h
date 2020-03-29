#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int TRANSFORM_COMPONENT = 1;

		class Transform : public iComponent
		{
		public:
			Transform();
			bool Deserialize(Json::Value& json) override;

			glm::mat4 transform;
		};
	}
}
