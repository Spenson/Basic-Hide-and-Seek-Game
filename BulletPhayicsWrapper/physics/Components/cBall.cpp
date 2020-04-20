/**
 * \file	cBall.cpp
 * \path	D:\GDP\Degenerate\DegenMyPhysicsWrapper\physics
 * \brief	Class Implementation File
 *
 * \desc	contains the implementation of cBall class
 */

#include "cBall.h"
#include "../nConvert.h"


namespace DegenBulletPhysicsWrapper
{
	/**
	 * \method		~cBall
	 * \fullname	DegenMyPhysicsWrapper::cBall::~cBall
	 * \brief		dtor
	 *
	 * \access		virtual public
	 *
	 * \desc		cleans up mBody and mShape
	 */
	cBall::~cBall()
	{
		mBody->setUserPointer(nullptr);
		delete mBody->getCollisionShape();
		delete mBody->getMotionState();
		delete mBody;
		mBody = nullptr;
	}

	/**
	 * \method		cBall
	 * \fullname	DegenMyPhysicsWrapper::cBall::cBall
	 * \brief		ctor
	 *
	 * \access		public
	 * \param		Degen::Physics::sSphereDef def
	 *
	 * \desc		creates Rigid body and sphere shape based off values in def
	 */
	cBall::cBall(Degen::Physics::sBallDef def)
	{
		btCollisionShape* shape = new btSphereShape(def.Radius);
		
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
		mBody->setSleepingThresholds(0,0);
		// impulse effected by mass velocity is dirrectly set
		//mBody->applyCentralImpulse(nConvert::ToBullet(def.Velocity));
		//
		
	}

	/**
	 * \method		GetTransform
	 * \fullname	DegenMyPhysicsWrapper::cBall::GetTransform
	 * \brief		mBody GetTransform wrapper
	 *
	 * \access		virtual public
	 * \return		glm::mat4 & transformOut
	 *
	 * \desc		Get the transform calculation for rendering the object
	 */
	void cBall::GetTransform(glm::mat4& transformOut)
	{
		btTransform transform;
		mBody->getMotionState()->getWorldTransform(transform);
		nConvert::ToGLM(transform, transformOut);
	}

	/**
	 * \method		ApplyForce
	 * \fullname	DegenMyPhysicsWrapper::cBall::ApplyForce
	 * \brief		mBody ApplyForce Wrapper
	 *
	 * \access		virtual public
	 * \return		void
	 * \param		const glm::vec3 & force
	 *
	 * \desc		add force to the acceleration of the object
	 */
	void cBall::ApplyForce(const glm::vec3& force)
	{
		mBody->activate(true);
		mBody->applyCentralForce(nConvert::ToBullet(force));
	}

	void cBall::ApplyImpulse(const glm::vec3& impulse)
	{
		mBody->activate(true);
		mBody->applyCentralImpulse(nConvert::ToBullet(impulse));
	}

	void cBall::SetEntityId(int id)
	{
		mBody->setUserIndex(id);
	}

	int cBall::GetEntityId()
	{
		return mBody->getUserIndex();
	}

	void cBall::SetSecondaryId(int id)
	{
		mBody->setUserIndex2(id);
	}

	int cBall::GetSecondaryId()
	{
		return mBody->getUserIndex2();
	}

	void cBall::AddToWorld(btDynamicsWorld* world)
	{
		world->addRigidBody(mBody);
	}

	void cBall::RemoveFromWorld(btDynamicsWorld* world)
	{
		world->removeRigidBody(mBody);
	}

	void cBall::SetPosition(const glm::vec3& position)
	{
		btTransform transform;
		transform.setIdentity();
		//mBody->getMotionState()->getWorldTransform(transform);
		transform.setOrigin(nConvert::ToBullet(position));
		mBody->setWorldTransform(transform);
		mBody->setLinearVelocity(btVector3(0,0,0));
		mBody->getMotionState()->setWorldTransform(transform);

	}


}
