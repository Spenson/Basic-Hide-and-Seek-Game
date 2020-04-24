#pragma once
#include "../Entity/cEntityManager.h"
#include <glm/vec3.hpp>


namespace Degen
{
	namespace Game
	{
		class HideAndSeek
		{
			std::vector<glm::vec3> pos;
			Entity::cEntity* player;
			Entity::cEntity* goblin;
			Entity::cEntity* statue;
			Entity::cEntity* camera;

			std::vector<Entity::cEntity*> fireflies;
			Entity::cEntity* tree_collisions;
			
		public:
			HideAndSeek();

			void AddEntity(Entity::cEntity* entity);
			void Update(double dt);

			void InteractFriend();
			void InteractStatue();
		};
	}
}
