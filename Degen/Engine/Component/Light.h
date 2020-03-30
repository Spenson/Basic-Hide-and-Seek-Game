#pragma once
#include "iComponent.h"
#include <glm.hpp>

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int LIGHT_COMPONENT = 5;

		class Light : public iComponent
		{
		public:
			enum LightType
			{
				point_light,
				spot_light,
				directional_light
			};
			
			Light();
			bool Deserialize(Json::Value& json) override;

			glm::vec4 diffuse;	
			glm::vec4 specular;
			float const_atten;
			float linear_atten;
			float quad_atten;
			float cutoff;
			glm::vec3 direction;
			glm::vec3 position;
			
			LightType type;
			float inner_angle;
			float outer_angle;

			bool on;

		};
	}
}
