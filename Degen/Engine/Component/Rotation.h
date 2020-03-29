#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int ROTATION_COMPONENT = 2;

		class Rotation : public iComponent
		{
		public:
			Rotation();
			bool Deserialize(Json::Value& json) override;

			glm::quat rotation;
		};

	}
}
