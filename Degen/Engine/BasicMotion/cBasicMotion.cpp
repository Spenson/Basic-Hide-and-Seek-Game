#include "cBasicMotion.h"
#include "../Component/BasicMotion.h"
#include "../Component/Position.h"
#include "../Component/Rotation.h"

namespace Degen
{
	namespace BasicMotion
	{
		void cBasicMotion::Update(double dt)
		{
			for(auto* ent: entities)
			{
				Component::BasicMotion* vel = dynamic_cast<Component::BasicMotion*>(ent->GetComponent(Component::BASIC_MOTION_COMPONENT));
				if(glm::length(vel->velocity) < 0.01f)
				{
					continue;
				}

				Component::Position* pos = dynamic_cast<Component::Position*>(ent->GetComponent(Component::POSITION_COMPONENT));
				pos->position += vel->velocity * (float)dt;

				Component::Rotation* rot = dynamic_cast<Component::Rotation*>(ent->GetComponent(Component::ROTATION_COMPONENT));
				if(rot)
				{
					rot->rotation = glm::quatLookAt(-glm::normalize(vel->velocity), glm::vec3(0.f,1.f,0.f));
				}
			}
		}
		void cBasicMotion::AddEntity(Entity::cEntity* entity)
		{
			if (entity->HasComponent(Component::BASIC_MOTION_COMPONENT) && entity->HasComponent(Component::POSITION_COMPONENT))
			{
				if (std::find(entities.begin(), entities.end(), entity) == entities.end())
				{
					entities.push_back(entity);
				}
			}
		}
	}
}

