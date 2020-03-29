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
			std::string texture;
			glm::vec4 diffuse_colour;
			glm::vec4 specular_colour;
			//glm::vec3 position;
			//glm::quat roatation;
			glm::vec3 scale;

			bool ignore_lighting;
			bool is_wireframe;
		};
	}
}
