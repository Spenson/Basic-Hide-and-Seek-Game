#include "Animation_New.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		Animation_New::Animation_New() : iComponent(ANIMATION_NEW_COMPONENT),
			playing_time(0),
			blending_time(0),
			play_till_end(false)
		{
		}
		bool Animation_New::Deserialize(Json::Value& json)
		{
			JsonHelp::Set(json["animation"], animation);
			JsonHelp::Set(json["play_till_end"], play_till_end);

			return true;
		}
	}
}
