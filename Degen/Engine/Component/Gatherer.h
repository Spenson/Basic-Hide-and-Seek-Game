#pragma once
#include "iComponent.h"
#include "../AI/Graph.h"

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
				wait,
				return_
			};
			state current_state = idle;
			float time;
			AI::Node* node;
			
			std::vector<AI::Node*> path;
			
		};
	}
}
