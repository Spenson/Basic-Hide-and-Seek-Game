#pragma once
#include "iComponent.h"
#include <glm.hpp>

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int RENDER_COMPONENT = 4;

		class Render : public iComponent
		{
		public:
			Render();
			bool Deserialize(Json::Value& json) override;

			std::string mesh; //Model
			
			std::string texture1;
			float texture1_amount;
			std::string texture2;
			float texture2_amount;
			bool is_cubemap_textures;
			
			glm::vec4 diffuse_colour;
			float diffuse_amount;
			glm::vec4 specular_colour;
			glm::vec3 scale;

			bool ignore_lighting;
			bool is_wireframe;
			bool cull_face_back;
		};
	}
}
