#pragma once
#include <glm.hpp>
#include "physics/interfaces/iTriggerRegionComponent.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "../iBulletComponent.h"


namespace DegenBulletPhysicsWrapper
{
	class cTriggerRegion :
		public Degen::Physics::iTriggerRegionComponent,
		public iBulletComponent
	{
	public:
		
		cTriggerRegion(const Degen::Physics::sTriggerRegionDef& def);
		virtual ~cTriggerRegion();
		
		void GetTransform(glm::mat4& transformOut) override;

		bool EntityIdIsTriggeredBy(int entityId) override;
		bool SecondaryIdIsTriggeredBy(int entityId) override;

		void SetEntityId(int id) override;
		int GetEntityId() override;
		void SetSecondaryId(int id) override;
		int GetSecondaryId() override;
		void AddToWorld(btDynamicsWorld* world) override;
		void RemoveFromWorld(btDynamicsWorld* world) override;
	private:
		btPairCachingGhostObject* mGhostObject;

		cTriggerRegion() = delete;
		cTriggerRegion(const cTriggerRegion& other) = delete;
		cTriggerRegion& operator=(const cTriggerRegion& other) = delete;
	};
}
