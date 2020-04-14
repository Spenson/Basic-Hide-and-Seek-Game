#pragma once

#include "iComponent.h"
#include "glm.hpp"
#include "physics/interfaces/iPhysicsComponent.h"

//gonna be different than other components

namespace Degen
{
	namespace Component
	{
		constexpr unsigned int PHYSICS_COMPONENT = 10;

		class Physics : public iComponent
		{
		public:
			Physics();
			bool Deserialize(Json::Value& json) override;

			Degen::Physics::iPhysicsComponent* comp;
			
		};

	}
}
