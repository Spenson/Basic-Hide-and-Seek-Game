#include "cCone.h"
#include "../nConvert.h"

namespace DegenBulletPhysicsWrapper
{
	cCone::~cCone()
	{
		mBody->setUserPointer(nullptr);
		delete mBody->getCollisionShape();
		delete mBody->getMotionState();
		delete mBody;
		mBody = nullptr;
	}
	cCone::cCone(Degen::Physics::sConeDef def)
	{
		btCollisionShape* shape = new btConeShape(def.Radius, def.Height * 2.f);

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(nConvert::ToBullet(def.Position));

		btScalar mass(def.Mass);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
		{
			shape->calculateLocalInertia(mass, localInertia);
		}

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
		rbInfo.m_restitution = 0.5f;

		mBody = new btRigidBody(rbInfo);
		mBody->setUserPointer(this);
	}
	void cCone::GetTransform(glm::mat4& transformOut)
	{
		btTransform transform;
		mBody->getMotionState()->getWorldTransform(transform);
		nConvert::ToGLM(transform, transformOut);
	}
	void cCone::ApplyForce(const glm::vec3& force)
	{
		mBody->activate(true);
		mBody->applyCentralForce(nConvert::ToBullet(force));
	}
	void cCone::ApplyImpulse(const glm::vec3& impulse)
	{
		mBody->activate(true);
		mBody->applyCentralImpulse(nConvert::ToBullet(impulse));
	}
	void cCone::SetEntityId(int id)
	{
		mBody->setUserIndex(id);
	}
	int cCone::GetEntityId()
	{
		return mBody->getUserIndex();
	}
	void cCone::SetSecondaryId(int id)
	{
		mBody->setUserIndex2(id);
	}
	int cCone::GetSecondaryId()
	{
		return mBody->getUserIndex2();
	}
	void cCone::AddToWorld(btDynamicsWorld* world)
	{
		world->addRigidBody(mBody);
	}
	void cCone::RemoveFromWorld(btDynamicsWorld* world)
	{
		world->removeRigidBody(mBody);
	}
}
