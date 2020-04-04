#pragma once
#include "iComponent.h"
#include "glm.hpp"

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int ANIMATION_COMPONENT = 9;

		class Animation : public iComponent
		{
		public:
			Animation();
			bool Deserialize(Json::Value& json) override;

			float playing_time;
			float blending_time;		//amount of playing_time blended for
			
			std::string default_animation; //idle
			float default_blend_time; //idle


			std::string playing_animation;
			float playing_animation_blend_time;		// playing_time to blend for
			bool play_through;		// play until finished or next animation is set (for stuff like emoji dances)
			bool lock_animation;	// manual stop only play until finished (for stuff like jump)

			std::string next_animation;		//for belending after this gets going
			float next_animation_blend_time;			// ignored till copied
			float next_animation_time;			// playing_time animation has played for
			bool next_play_through;		// play until finished or next animation is set (for stuff like emoji dances)
			bool next_lock_animation;	// play until finished (for stuff like jump)
			

			glm::mat4 relitive_matrix;
			std::vector<glm::mat4> bone_transforms;
		};

	}
}
