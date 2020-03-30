#include "Camera.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		Camera::Camera() : iComponent(CAMERA_COMPONENT),
			active(false),
			transition_time(1.f),
			offset(),
			lock_x(false),
			lock_y(false),
			lock_z(false),
			lock_pitch(false),
			pitch(0.f),
			max_pitch(89.9f),
			min_pitch(89.9f),
			lock_yaw(false),
			yaw(0.f),
			max_yaw(181.f),
			min_yaw(-181.f),
			distance(10.f)
		{
		}

		bool Camera::Deserialize(Json::Value& json)
		{
			JsonHelp::Set(json["active"], active);
			JsonHelp::Set(json["transition_time"], transition_time);
			JsonHelp::Set(json["pitch"], pitch);
			JsonHelp::Set(json["max_pitch"], max_pitch);
			JsonHelp::Set(json["min_pitch"], min_pitch);
			JsonHelp::Set(json["yaw"], yaw);
			JsonHelp::Set(json["max_yaw"], max_yaw);
			JsonHelp::Set(json["min_yaw"], min_yaw);
			JsonHelp::Set(json["distance"], distance);
			return true;
		}

	}
}

