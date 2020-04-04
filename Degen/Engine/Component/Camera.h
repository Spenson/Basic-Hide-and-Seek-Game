#pragma once
#include "iComponent.h"
#include <glm.hpp>
namespace Degen
{
	namespace Component
	{
		constexpr unsigned int CAMERA_COMPONENT = 6;
		class Camera: public iComponent
		{
		public:
			Camera();
			bool Deserialize(Json::Value& json) override;

			bool active;
			float transition_time;

			glm::vec3 offset;
			bool lock_x;
			bool lock_y;
			bool lock_z;

			bool lock_pitch;
			float pitch;
			float max_pitch;
			float min_pitch;

			bool lock_yaw;
			float yaw;
			float max_yaw;
			float min_yaw;
			
			float distance;
			float max_distance;
			float min_distance;
		};
	}
}
