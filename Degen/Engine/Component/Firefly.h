#pragma once
#include "iComponent.h"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int FIREFLY_COMPONENT = 7;
		class Firefly: public iComponent
		{
		public:
			Firefly();
			bool Deserialize(Json::Value& json) override;

			
		};
	}
}
