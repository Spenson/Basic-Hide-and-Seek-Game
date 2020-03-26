#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int TRANSFORM_TYPE = 1;

		class Transform : public iComponent
		{
		public:
			Transform();
			bool Serialize(Json::Value& json) override;

			glm::mat4 transform;
		};
	}
}
