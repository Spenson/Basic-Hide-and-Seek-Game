#pragma once
#include "iPhysicsComponent.h"

namespace Degen
{

namespace Physics
{
	class iCollisionListener
	{
	public:
		virtual ~iCollisionListener() {};
		virtual void Collide(iPhysicsComponent* compA, iPhysicsComponent* compB) = 0;
		virtual void Collide(iPhysicsComponent* compA, iPhysicsComponent* compB, glm::vec3 collision_point) = 0;
	};
}
}