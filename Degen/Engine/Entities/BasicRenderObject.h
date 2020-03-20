#pragma once
#include "iGameObject.h"
#include <glm.hpp>
namespace Degen
{
	namespace Object
	{
		int constexpr BASIC_RENDER_TYPE = 0b0000'0000;
		
		class BasicRenderObject: public iGameObject
		{
		public:
			BasicRenderObject() : iGameObject(BASIC_RENDER_TYPE) {}
			BasicRenderObject(std::string& name) : iGameObject(BASIC_RENDER_TYPE, name) {}

			glm::vec3 position;
			glm::vec4 diffuse_colour;
		};
	}
}
