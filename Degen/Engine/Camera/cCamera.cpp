#include "cCamera.h"
#include "../Component/Camera.h"
#include "../Component/Position.h"
#include "../Component/Transform.h"
#include "../Globals.h"

namespace Degen
{
	namespace Camera
	{
		void cCamera::Update(double dt)
		{
			bool cam_set = false;
			Component::Camera* cam;
			for (auto* entity : mEntities)
			{
				cam = dynamic_cast<Component::Camera*>(entity->GetComponent(Component::CAMERA_COMPONENT));
				if (cam_set)
				{
					cam->active = false;
					continue;
				}
				if (cam->active)
				{
					if (current_entity == entity)
					{
						continue;
					}
					previous_entity = current_entity;
					current_entity = entity;
					cam_set = true;
					if (previous_entity)
						dynamic_cast<Component::Camera*>(previous_entity->GetComponent(Component::CAMERA_COMPONENT))->active = false;
					transition_time_remaining = cam->transition_time;
				}
			}

			if (!current_entity) return;

			if (transition_time > 0.f)
			{

			}

			cam = dynamic_cast<Component::Camera*>(current_entity->GetComponent(Component::CAMERA_COMPONENT));


			if (cam->pitch > cam->max_pitch) { cam->pitch = cam->max_pitch; }
			if (cam->pitch < cam->min_pitch) { cam->pitch = cam->min_pitch; }
			
			if (cam->yaw > 360.f) { cam->yaw -= 360.f; }
			if (cam->yaw < -360.f) { cam->yaw += 360.f; }
			
			if (cam->yaw > cam->max_yaw) { cam->yaw = cam->max_yaw; }
			if (cam->yaw < cam->min_yaw) { cam->yaw = cam->min_yaw; }



			glm::vec3 up(0.f, 1.f, 0.f);
			glm::vec3 target(0.f);

			if (current_entity->HasComponent(Component::POSITION_COMPONENT))
			{
				target = dynamic_cast<Component::Position*>(current_entity->GetComponent(Component::POSITION_COMPONENT))->position;
			}
			else
			{
				glm::mat4 transform = dynamic_cast<Component::Transform*>(current_entity->GetComponent(Component::TRANSFORM_COMPONENT))->transform;
				target = glm::vec3(transform[3].x, transform[3].y, transform[3].z);
			}

			if (cam->lock_x) target.x = cam->offset.x;
			else  target.x += cam->offset.x;
			if (cam->lock_y) target.y = cam->offset.y;
			else  target.y += cam->offset.y;
			if (cam->lock_z) target.z = cam->offset.z;
			else  target.z += cam->offset.z;

			glm::vec3 position(0.f, 0.f, 1.f);
			glm::quat rotation(glm::radians(glm::vec3(cam->pitch, cam->yaw, 0.f)));
			position = rotation * position;
			position *= cam->distance;
			position += target;

			View->target = target;
			View->position = position;
			View->up = up;

		}
		void cCamera::AddEntity(Entity::cEntity* entity)
		{
			if (entity->HasComponent(Component::CAMERA_COMPONENT) &&
				(entity->HasComponent(Component::TRANSFORM_COMPONENT) || entity->HasComponent(Component::POSITION_COMPONENT)))
			{
				mEntities.push_back(entity);
			}
		}
	}
}
