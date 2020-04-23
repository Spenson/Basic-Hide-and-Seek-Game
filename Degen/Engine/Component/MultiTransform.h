#pragma once
#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int MULTI_TRANSFORM_COMPONENT = 11;

		class MultiTransform : public iComponent
		{
		public:
			MultiTransform();
			bool Deserialize(Json::Value& json) override;

			std::vector<glm::mat4> transforms;
		};
	}
}
