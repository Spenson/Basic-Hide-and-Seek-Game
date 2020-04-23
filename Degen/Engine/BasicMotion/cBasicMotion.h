#pragma once
#include "../Entity/cEntity.h"

namespace Degen
{
	namespace BasicMotion
	{
		class cBasicMotion
		{
		public:
			std::vector<Entity::cEntity*> moving_entities;
			void Update(double dt);
			void AddEntity(Entity::cEntity* entity);
		};
	}
}
