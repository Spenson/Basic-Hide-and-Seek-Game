#pragma once
#include <string>
#include "../Entity/cEntity.h"

namespace Degen
{
	namespace AI
	{
		class cIntelligence
		{
		public:
			cIntelligence(std::string map_file);
			void Update(double dt);
			void AddEntity(Entity::cEntity* entity);

			std::vector<Entity::cEntity*> entities;
		};
	}
}
