#pragma once
#include "iGameObject.h"
#include <glm.hpp>
namespace Degen
{
	namespace Object
	{
		int constexpr RENDER_ONLY_TYPE = 0b0000'0000;
		
		class cRenderOnlyObject: public iGameObject
		{
		public:
			cRenderOnlyObject() : iGameObject(RENDER_ONLY_TYPE) {}
			cRenderOnlyObject(std::string& name) : iGameObject(RENDER_ONLY_TYPE, name) {}

			glm::mat4 Transform();

			std::string mesh; //Model
			glm::vec4 diffuse_colour;
			glm::vec4 specular_colour;
			glm::vec3 position;
			glm::quat roatation;
			glm::vec3 scale;

			bool ignore_lighting;
			bool is_wireframe;
		};
	}
}
