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
			//glm::vec4 atten;	// x = constant, y = linear, z = quadratic, w = DistanceCutOff
			glm::vec3 direction;// relitive to position. must nbe normalized

			LightType type;
			float inner_angle;
			float outer_angle;

			bool on;

		};
	}
}
