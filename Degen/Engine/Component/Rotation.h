#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int ROTATION_TYPE = 2;

		class Rotation : public iComponent
		{
		public:
			Rotation();
			bool Serialize(Json::Value& json) override;

			glm::quat rotation;
		};

	}
}
