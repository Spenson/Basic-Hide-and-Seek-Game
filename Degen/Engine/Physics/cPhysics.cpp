#include "cPhysics.h"
#include "../Globals.h"
#include "../Component/Physics.h"
#include "../Component/Transform.h"
#include "../Component/Position.h"

namespace Degen
{
	namespace Physics
	{
		cPhysics::cPhysics(glm::vec3 gravity)
		{
			mWorld = PhysicsFactory->CreateWorld();
			mWorld->SetGravity(gravity);
		}
		cPhysics::~cPhysics()
		{
			delete mWorld;
		}

		void cPhysics::AddEntity(Entity::cEntity* entity)
		{
			Component::Physics* phys = dynamic_cast<Component::Physics*>(entity->GetComponent(Component::PHYSICS_COMPONENT));

			if (phys)
			{
				if (std::find(entities.begin(), entities.end(), entity) == entities.end())
				{
					entities.push_back(entity);
					mWorld->AddComponent(phys->comp);
				}
			}
		}
		void cPhysics::Update(double dt)
		{
			mWorld->Update(dt);

			for (auto entity : entities)
			{
				Component::Transform* trans = dynamic_cast<Component::Transform*>(entity->GetComponent(Component::TRANSFORM_COMPONENT));
				if (trans)
				{
					Component::Physics* phys = dynamic_cast<Component::Physics*>(entity->GetComponent(Component::PHYSICS_COMPONENT));

					phys->comp->GetTransform(trans->transform);

					continue;
				}
			}
		}
	}
}
