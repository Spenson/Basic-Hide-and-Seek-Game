#pragma once
#include "../Entity/cEntityManager.h"
#include <physics/interfaces/iPhysicsFactory.h>
#include <glm.hpp>

namespace Degen
{
	
	namespace Physics
	{
		class cPhysics
		{
		public:
			cPhysics(glm::vec3 gravity);
			~cPhysics();
			void AddEntity(Entity::cEntity* entity);
			void Update(double dt);

			std::vector<Entity::cEntity*> entities;
			iPhysicsWorld* mWorld;
		};
	}
}
