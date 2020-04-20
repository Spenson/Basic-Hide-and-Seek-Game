#include "PinBall.h"
#include "../Globals.h"
#include "../Component/Physics.h"
#include "../Component/Position.h"
#include "../Component/Render.h"


namespace Degen
{
	namespace Game
	{
		Pinball::Pinball() : mInplay(false),
			mLives(3),
			mBall(nullptr),
			mTunnel_wall(nullptr),
			mReset_zone(nullptr),
			mInplay_zone(nullptr)
		{
		}

		Pinball::~Pinball()
		{
		}

		void Pinball::Update(double dt)
		{
			if (mInplay)
			{
				Component::Physics* phys_comp = dynamic_cast<Component::Physics*>(mReset_zone->GetComponent(Component::PHYSICS_COMPONENT));
				Physics::iTriggerRegionComponent* trigger = dynamic_cast<Physics::iTriggerRegionComponent*>(phys_comp->comp);
				if (trigger->SecondaryIdIsTriggeredBy(6))
				{
					mLives--;
					dynamic_cast<Physics::iBoxComponent*>(dynamic_cast<Component::Physics*>(mTunnel_wall->GetComponent(Component::PHYSICS_COMPONENT))->comp)->SetPosition(glm::vec3(15, -3, -9));

					dynamic_cast<Physics::iBallComponent*>(dynamic_cast<Component::Physics*>(mBall->GetComponent(Component::PHYSICS_COMPONENT))->comp)->SetPosition(glm::vec3(16, -2.0, -10.0));

					if (!mLives)
					{
						GameOver();
					}

					mInplay = false;
				}
			}
			else
			{
				Component::Physics* phys_comp = dynamic_cast<Component::Physics*>(mInplay_zone->GetComponent(Component::PHYSICS_COMPONENT));
				Physics::iTriggerRegionComponent* trigger = dynamic_cast<Physics::iTriggerRegionComponent*>(phys_comp->comp);
				if (trigger->SecondaryIdIsTriggeredBy(6))
				{
					dynamic_cast<Physics::iBoxComponent*>(dynamic_cast<Component::Physics*>(mTunnel_wall->GetComponent(Component::PHYSICS_COMPONENT))->comp)->SetPosition(glm::vec3(15, -3, -5));
					mInplay = true;
				}
			}

			TextRenderer->AddText("Score: " + std::to_string(mScore), 25.0f, WINDOW_HEIGHT * 1.7, 1.1f, glm::vec3(.9f, .9f, .9f));
			TextRenderer->AddText("Lives: " + std::to_string(mLives), 25.0f, WINDOW_HEIGHT * 1.6, 1.0f, glm::vec3(.8f, .8f, .8f));
		}
		void Pinball::AddEntity(Entity::cEntity* entity)
		{
			if (entity->user_id == 6)
			{
				mBall = entity;
			}
			else if (entity->user_id == 1 || entity->user_id == 2)
			{
				mBumpers_Panels.push_back(entity);
			}
			else if (entity->user_id == 3)
			{
				mTunnel_wall = entity;
			}
			else if (entity->user_id == 4)
			{
				mReset_zone = entity;
			}
			else if (entity->user_id == 5)
			{
				mInplay_zone = entity;
			}



		}
		void Pinball::GameOver()
		{
			//High score stuff if I get to it

			mScore = 0;
			mLives = 3;
		}

		void Pinball::Collide(Physics::iPhysicsComponent* compA, Physics::iPhysicsComponent* compB, glm::vec3 collision_point)
		{
			if (compA->GetSecondaryId() == 6)
			{
				if (compB->GetSecondaryId() == 1)
				{
					mScore += 10;
					glm::mat4 temp;
					compA->GetTransform(temp);
					glm::vec3 ball_pos(temp[3].x, temp[3].y, temp[3].z);
					glm::vec3 forceDir = glm::normalize(ball_pos - collision_point);
					dynamic_cast<Physics::iBallComponent*>(compA)->ApplyImpulse(forceDir * 30.f);
				}
				else if (compB->GetSecondaryId() == 2)
				{
					mScore += 15;
					glm::mat4 temp;
					compA->GetTransform(temp);
					glm::vec3 ball_pos(temp[3].x, temp[3].y, temp[3].z);
					glm::vec3 forceDir = glm::normalize(ball_pos - collision_point);
					dynamic_cast<Physics::iBallComponent*>(compA)->ApplyImpulse(forceDir * 35.f);
				}

			}
			else if (compB->GetSecondaryId() == 6)
			{
				if (compB->GetSecondaryId() == 1)
				{
					mScore += 10;
				}
				else if (compB->GetSecondaryId() == 2)
				{
					mScore += 15;
				}

			}
		}

		void Pinball::Collide(Physics::iPhysicsComponent* compA, Physics::iPhysicsComponent* compB)
		{
			
		}
	}
}
