#include "cBasicMotion.h"
#include "../Component/Motion.h"
#include "../Component/Position.h"
#include "../Component/Rotation.h"
#include "../Component/Transform.h"
#include "../Component/MultiTransform.h"
#include "../Component/Radius2D.h"
#include "../Component/AABB2D.h"

namespace Degen
{
	namespace BasicMotion
	{
		void cBasicMotion::Update(double dt)
		{
			for (auto* ent : movable_entities)
			{
				Component::Position* position = dynamic_cast<Component::Position*>(ent->GetComponent(Component::POSITION_COMPONENT));
				Component::Motion* motion = dynamic_cast<Component::Motion*>(ent->GetComponent(Component::MOTION_COMPONENT));

				if (glm::length(motion->velocity) < 0.01) continue;
				
				if (glm::length(motion->velocity) > motion->max_velocity)
				{
					motion->velocity = glm::normalize(motion->velocity) * motion->max_velocity;
				}

				position->position += motion->velocity * (float)dt;

				Component::Radius2D* radius = dynamic_cast<Component::Radius2D*>(ent->GetComponent(Component::RADIUS_2D_COMPONENT));
				if (radius)
				{
					CheckBoundsXZ(radius->radius, position->position);
					CheckOtherObjects(radius->radius, position->position, ent->unique_id);
				}
			}
		}
		void cBasicMotion::AddEntity(Entity::cEntity* entity)
		{
			if (entity->HasComponent(Component::MOTION_COMPONENT) && entity->HasComponent(Component::POSITION_COMPONENT))
			{
				if (std::find(movable_entities.begin(), movable_entities.end(), entity) == movable_entities.end())
				{
					movable_entities.push_back(entity);
				}
			}

			if ((entity->HasComponent(Component::RADIUS_2D_COMPONENT) || entity->HasComponent(Component::AABB_2D_COMPONENT))
				&& (entity->HasComponent(Component::TRANSFORM_COMPONENT) || entity->HasComponent(Component::MULTI_TRANSFORM_COMPONENT) || entity->HasComponent(Component::POSITION_COMPONENT)))
			{
				if (std::find(collision_entities.begin(), collision_entities.end(), entity) == collision_entities.end())
				{
					collision_entities.push_back(entity);
				}
			}
		}

		void cBasicMotion::CheckBoundsXZ(float& radius, glm::vec3& position)
		{
			if (position.x + radius > max.x) position.x = max.x - radius;
			if (position.z + radius > max.z) position.z = max.z - radius;

			if (position.x - radius < min.x) position.x = min.x + radius;
			if (position.z - radius < min.z) position.z = min.z + radius;
			
			//if (position.y + radius > max.y) position.y = max.y - radius;
			//if (position.y - radius < min.y) position.y = min.y + radius;
			
		}

		void cBasicMotion::CheckOtherObjects(float& radius, glm::vec3& position, unsigned id)
		{
			for (auto* other : collision_entities)
			{
				if(id == other->unique_id) continue;
				
				Component::Position* other_pos = dynamic_cast<Component::Position*>(other->GetComponent(Component::POSITION_COMPONENT));
				if (other_pos)
				{
					Component::Radius2D* other_rad = dynamic_cast<Component::Radius2D*>(other->GetComponent(Component::RADIUS_2D_COMPONENT));
					if (other_rad)
					{
						CircleCircle(radius, position, other_rad->radius, other_pos->position);
						continue;
					}

					Component::AABB2D* other_AABB = dynamic_cast<Component::AABB2D*>(other->GetComponent(Component::AABB_2D_COMPONENT));
					if (other_AABB)
					{
						CircleAABB(radius, position, other_AABB->half_extents, other_pos->position);
					}

					continue;
				}

				Component::Transform* other_transform = dynamic_cast<Component::Transform*>(other->GetComponent(Component::TRANSFORM_COMPONENT));
				if (other_transform)
				{
					Component::Radius2D* other_rad = dynamic_cast<Component::Radius2D*>(other->GetComponent(Component::RADIUS_2D_COMPONENT));
					if (other_rad)
					{
						CircleCircle(radius, position, other_rad->radius, glm::vec3(other_transform->transform[3].x, other_transform->transform[3].y, other_transform->transform[3].z));
						continue;
					}

					Component::AABB2D* other_AABB = dynamic_cast<Component::AABB2D*>(other->GetComponent(Component::AABB_2D_COMPONENT));
					if (other_AABB)
					{
						CircleAABB(radius, position, other_AABB->half_extents, glm::vec3(other_transform->transform[3].x, other_transform->transform[3].y, other_transform->transform[3].z));
					}

					continue;
				}

				Component::MultiTransform* other_mt = dynamic_cast<Component::MultiTransform*>(other->GetComponent(Component::MULTI_TRANSFORM_COMPONENT));
				if (other_mt)
				{
					
					Component::Radius2D* other_rad = dynamic_cast<Component::Radius2D*>(other->GetComponent(Component::RADIUS_2D_COMPONENT));
					if (other_rad)
					{
						for (glm::mat4 transform : other_mt->transforms)
						{
							CircleCircle(radius, position, other_rad->radius, glm::vec3(transform[3].x, transform[3].y, transform[3].z));
						}

						continue;
					}

					Component::AABB2D* other_AABB = dynamic_cast<Component::AABB2D*>(other->GetComponent(Component::AABB_2D_COMPONENT));
					if (other_AABB)
					{
						for (glm::mat4 transform : other_mt->transforms)
						{
							CircleAABB(radius, position, other_AABB->half_extents, glm::vec3(transform[3].x, transform[3].y, transform[3].z));
						}
					}

					continue;
				}



			}

		}

		void cBasicMotion::CircleCircle(float& radius, glm::vec3& position, float radius_other, glm::vec3 position_other)
		{
			glm::vec2 from_other = glm::vec2(position.x, position.z) - glm::vec2(position_other.x, position_other.z);
			float adgust = radius + radius_other - glm::length(from_other);


			if (adgust > 0.f)
			{
				from_other = glm::normalize(from_other);
				position += glm::vec3(from_other.x * adgust, 0.f, from_other.y * adgust);
			}

		}

		void cBasicMotion::CircleAABB(float& radius, glm::vec3& position, glm::vec2 half_lengths, glm::vec3 position_other)
		{
			glm::vec2 pos2d = glm::vec2(position.x, position.z);
			glm::vec2 pos2dother = glm::vec2(position_other.x, position_other.z);

			glm::vec2 difference = pos2d - pos2dother;

			glm::vec2 clamped = glm::clamp(difference, -half_lengths, half_lengths);
			glm::vec2 closest = pos2dother + clamped;

			difference = closest - pos2d;

			float adgust = radius - glm::length(difference);

			if (adgust > 0.f)
			{
				difference = glm::normalize(difference);
				position -= glm::vec3(difference.x * adgust, 0.f, difference.y * adgust);
			}


		}




	}
}