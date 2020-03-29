#include "Light.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		Light::Light() :iComponent(LIGHT_COMPONENT),
		diffuse(1.f),
		specular(1.f),
		const_atten(0.f),
		linear_atten(0.f),
		quad_atten(0.f),
		cutoff(1000.f),
		direction(0.f,-1.f,0.f),
		type(point_light),
		inner_angle(5.f),
		outer_angle(10.f),
		on(false) {}

		bool Light::Deserialize(Json::Value& json)
		{
			JsonHelp::Set(json["diffuse"], diffuse);
			JsonHelp::Set(json["specular"], specular);
			JsonHelp::Set(json["const_atten"], const_atten);
			JsonHelp::Set(json["linear_atten"], linear_atten);
			JsonHelp::Set(json["quad_atten"], quad_atten);
			JsonHelp::Set(json["cutoff"], cutoff);
			JsonHelp::Set(json["direction"], direction);
			int x = 0;
			JsonHelp::Set(json["type"], x);
			type = (LightType)x;
			JsonHelp::Set(json["inner_angle"], inner_angle);
			JsonHelp::Set(json["outer_angle"], outer_angle);
			JsonHelp::Set(json["on"], on);
			
			return true;
		}


	}
}
