#pragma once
#include "../Entity/cEntity.h"
#include <glm.hpp>
#include "../Component/Position.h"

namespace Degen
{
	namespace BasicMotion
	{
		class cBasicMotion
		{
		public:
			glm::vec3 max;
			glm::vec3 min;
			
			std::vector<Entity::cEntity*> movable_entities;
			std::vector<Entity::cEntity*> collision_entities;
			void Update(double dt);
			void AddEntity(Entity::cEntity* entity);
		private:
			void CheckBoundsXZ(float& radius, glm::vec3& position);
			void CheckBoundsY(float& radius, glm::vec3& position);
			void CheckOtherObjects(float& radius, glm::vec3& position, unsigned id);

			void CircleCircle(float& radius, glm::vec3& position, float radius_other, glm::vec3 position_other);
			void CircleAABB(float& radius, glm::vec3& position, glm::vec2 half_lengths, glm::vec3 position_other);
		};
	}
}
