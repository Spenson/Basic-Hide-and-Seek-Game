#include "cBox.h"
#include "../nConvert.h"

namespace DegenBulletPhysicsWrapper
{
	cBox::~cBox()
	{
		mBody->setUserPointer(nullptr);
		delete mBody->getCollisionShape();
		delete mBody->getMotionState();
		delete mBody;
		mBody = nullptr;
	}
	cBox::cBox(Degen::Physics::sBoxDef def)
	{
		btCollisionShape* shape = new btBoxShape(nConvert::ToBullet(def.Size));
		
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(nConvert::ToBullet(def.Position));
		transform.setRotation(nConvert::ToBullet(def.Rotation));
		

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
	void cBox::GetTransform(glm::mat4& transformOut)
	{
		btTransform transform;
		mBody->getMotionState()->getWorldTransform(transform);
		nConvert::ToGLM(transform, transformOut);
	}
	void cBox::ApplyForce(const glm::vec3& force)
	{
		mBody->activate(true);
		mBody->applyCentralForce(nConvert::ToBullet(force));
	}
	void cBox::ApplyImpulse(const glm::vec3& impulse)
	{
		mBody->activate(true);
		mBody->applyCentralImpulse(nConvert::ToBullet(impulse));
	}
	void cBox::SetEntityId(int id)
	{
		mBody->setUserIndex(id);
	}
	int cBox::GetEntityId()
	{
		return mBody->getUserIndex();
	}
	void cBox::SetSecondaryId(int id)
	{
		mBody->setUserIndex2(id);
	}
	int cBox::GetSecondaryId()
	{
		return mBody->getUserIndex2();
	}
	void cBox::AddToWorld(btDynamicsWorld* world)
	{
		world->addRigidBody(mBody);
	}
	void cBox::RemoveFromWorld(btDynamicsWorld* world)
	{
		world->removeRigidBody(mBody);
	}


	void cBox::SetPosition(const glm::vec3& position)
	{
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(nConvert::ToBullet(position));
		mBody->setWorldTransform(transform);
		mBody->getMotionState()->setWorldTransform(transform);

	}
}
