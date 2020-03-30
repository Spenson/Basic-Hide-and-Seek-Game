#pragma once
#include "iComponent.h"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int GATHERER_COMPONENT = 7;
		class Gatherer: public iComponent
		{
		public:
			Gatherer();
			bool Deserialize(Json::Value& json) override;

			enum state
			{
				idle,
				search,
				gather,
				return_
			};
			state current_state = idle;
			float time;
			std::vector<unsigned> path;
			
		};
	}
}
