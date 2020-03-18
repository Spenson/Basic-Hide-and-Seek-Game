#pragma once
#include "iGameObject.h"
#include <glm.hpp>
namespace Degen
{
	namespace Object
	{
		
		
		class BasicRenderObject: public iGameObject
		{
		public:
			int const TYPE = 0b0000'0000;
			BasicRenderObject() : iGameObject(TYPE) {}
			BasicRenderObject(std::string& name) : iGameObject(TYPE, name) {}

			glm::vec3 position;
			glm::vec4 diffuse_colour;
		};
	}
}
