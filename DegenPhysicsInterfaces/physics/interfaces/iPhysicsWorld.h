#pragma once
#include <glm.hpp>
#include "iPhysicsComponent.h"
#include "iCollisionListener.h"

namespace Degen
{

namespace Physics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}

		virtual void SetCollisionListener(iCollisionListener* listener) = 0;
		virtual void SetGravity(glm::vec3 gravity) = 0;
		virtual void Update(float dt) = 0;
		virtual bool AddComponent(iPhysicsComponent* component) = 0;
		virtual bool RemoveComponent(iPhysicsComponent* component) = 0;
	};
}
}