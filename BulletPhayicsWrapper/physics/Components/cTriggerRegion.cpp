#include "cTriggerRegion.h"
#include "../nConvert.h"

namespace DegenBulletPhysicsWrapper
{
	cTriggerRegion::cTriggerRegion(const Degen::Physics::sTriggerRegionDef& def)
	{
		mGhostObject = new btPairCachingGhostObject();
		mGhostObject->setUserPointer(this);

		mGhostObject->setCollisionShape(new btBoxShape(nConvert::ToBullet(def.Size)));

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(nConvert::ToBullet(def.Position));
		mGhostObject->setWorldTransform(transform);

		mGhostObject->setCollisionFlags(mGhostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

		
	}

	cTriggerRegion::~cTriggerRegion()
	{
		mGhostObject->setUserPointer(nullptr);
		delete mGhostObject->getCollisionShape();
		delete mGhostObject;
		mGhostObject = nullptr;
	}

	void cTriggerRegion::GetTransform(glm::mat4& transformOut)
	{
		btTransform transform(mGhostObject->getWorldTransform());
		nConvert::ToGLM(transform, transformOut);
	}

	bool cTriggerRegion::IsTriggeredBy(int entityId)
	{
		int triggered_count = mGhostObject->getNumOverlappingObjects();
		for(int idx = 0; idx < triggered_count; idx++)
		{
			if(mGhostObject->getOverlappingObject(idx)->getUserIndex() == entityId)
			{
				return true;
			}
		}
		return false;
	}

	void cTriggerRegion::SetEntityId(int id)
	{
		mGhostObject->setUserIndex(id);
	}

	int cTriggerRegion::GetEntityId()
	{
		return mGhostObject->getUserIndex();
	}

	void cTriggerRegion::SetSecondaryId(int id)
	{
		mGhostObject->setUserIndex2(id);
	}

	int cTriggerRegion::GetSecondaryId()
	{
		return mGhostObject->getUserIndex2();
	}

	void cTriggerRegion::AddToWorld(btDynamicsWorld* world)
	{
		world->addCollisionObject(mGhostObject);
	}

	void cTriggerRegion::RemoveFromWorld(btDynamicsWorld* world)
	{
		world->removeCollisionObject(mGhostObject);
	}
}
