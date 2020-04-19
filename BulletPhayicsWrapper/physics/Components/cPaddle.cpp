#include "cPaddle.h"
#include "../nConvert.h"

namespace DegenBulletPhysicsWrapper
{
	cPaddle::~cPaddle()
	{
		delete mConstraint;
		mConstraint = 0;
		mBody->setUserPointer(nullptr);
		delete mBody->getCollisionShape();
		delete mBody->getMotionState();
		delete mBody;
		mBody = nullptr;
	}
	cPaddle::cPaddle(Degen::Physics::sPaddleDef def)
	{
		btCollisionShape* shape = new btBoxShape(nConvert::ToBullet(def.Size));

		
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
		
		if(def.HingeOnPositiveX)
		{
			mConstraint = new btHingeConstraint(*mBody, btVector3(def.Size.x * 1.f,0.f,0.f), btVector3(0.f,1.f,0.f));
			mConstraint->setLimit(-.3f, .4f);
		}
		else
		{
			mConstraint = new btHingeConstraint(*mBody, btVector3(def.Size.x * -1.f, 0.f, 0.f), btVector3(0.f, 1.f, 0.f));
			mConstraint->setLimit(-.4f, .3f);
		}
		
	}
	void cPaddle::GetTransform(glm::mat4& transformOut)
	{
		btTransform transform;
		mBody->getMotionState()->getWorldTransform(transform);
		nConvert::ToGLM(transform, transformOut);
	}
	void cPaddle::ApplyForce(const glm::vec3& force)
	{
		mBody->activate(true);
		mBody->applyCentralForce(nConvert::ToBullet(force));
	}
	void cPaddle::ApplyImpulse(const glm::vec3& impulse)
	{
		mBody->activate(true);
		mBody->applyCentralImpulse(nConvert::ToBullet(impulse));
	}
	void cPaddle::Hit(const glm::vec3& force)
	{
		mBody->activate(true);
		mBody->applyForce(nConvert::ToBullet(force), -mConstraint->getAFrame().getOrigin());
	}
	void cPaddle::SetEntityId(int id)
	{
		mBody->setUserIndex(id);
	}
	int cPaddle::GetEntityId()
	{
		return mBody->getUserIndex();
	}
	void cPaddle::SetSecondaryId(int id)
	{
		mBody->setUserIndex2(id);
	}
	int cPaddle::GetSecondaryId()
	{
		return mBody->getUserIndex2();
	}
	void cPaddle::AddToWorld(btDynamicsWorld* world)
	{
		world->addRigidBody(mBody);
		world->addConstraint(mConstraint);
	}
	void cPaddle::RemoveFromWorld(btDynamicsWorld* world)
	{
		world->removeRigidBody(mBody);
		world->removeConstraint(mConstraint);
	}
}
