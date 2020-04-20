#pragma once
#include "../Entity/cEntityManager.h"
#include "physics/interfaces/iCollisionListener.h"

namespace Degen
{
	namespace Game
	{
		class Pinball : public Physics::iCollisionListener
		{
		public:
			Pinball();
			virtual ~Pinball();
			void Update(double dt);
			void AddEntity(Entity::cEntity* entity);
			void Collide(Physics::iPhysicsComponent* compA, Physics::iPhysicsComponent* compB) override;
		private:
			void GameOver();
		public:
			void Collide(Physics::iPhysicsComponent* compA, Physics::iPhysicsComponent* compB,
				glm::vec3 collision_point) override;
		private:
			bool mInplay;
			int mLives;
			unsigned mScore;
			std::vector<Entity::cEntity*> mBumpers_Panels;
			Entity::cEntity* mBall;
			Entity::cEntity* mTunnel_wall;
			Entity::cEntity* mReset_zone;
			Entity::cEntity* mInplay_zone;

		};
	}
}
