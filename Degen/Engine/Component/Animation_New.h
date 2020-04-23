#pragma once

#pragma once
#include "iComponent.h"
#include "glm.hpp"
#include <queue>

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int ANIMATION_NEW_COMPONENT = 10;

		struct animation_info
		{
			std::string animation;
			double blend_for;
			bool play_till_end;
		};
		
		class Animation_New : public iComponent
		{
		public:
			Animation_New();
			bool Deserialize(Json::Value& json) override;

			double playing_time;
			double blending_time;		//amount of playing_time blended for

			std::string animation;
			bool play_till_end;

			std::deque<animation_info> animation_queue;
			
			std::vector<glm::mat4> bone_transforms;
		};

	}
}
