#pragma once
#include <glm.hpp>
#include "../Entity/cEntity.h"

namespace Degen
{
	namespace Camera
	{

		class cCamera
		{
		public:
			Entity::cEntity* current_entity;
			Entity::cEntity* previous_entity;
			float transition_time_remaining;
			float transition_time;
			
			float max_yaw;
			float min_yaw;
			float max_pitch;
			float min_pitch;
			
			
			void Update(double dt);
			void AddEntity(Entity::cEntity* entity);
		private:
			std::vector<Entity::cEntity*> mEntities;
		};
	}
}