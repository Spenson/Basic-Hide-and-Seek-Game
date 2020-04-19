#include "cLauncher.h"
#include "../nConvert.h"

namespace DegenBulletPhysicsWrapper
{

	cLauncher::~cLauncher()
	{
		delete mSpring;
		mSpring = 0;
		mBody->setUserPointer(nullptr);
		delete mBody->getCollisionShape();
		delete mBody->getMotionState();
		delete mBody;
		mBody = nullptr;
	}
	cLauncher::cLauncher(Degen::Physics::sLauncherDef def)
	{
		btCollisionShape* shape = new btCylinderShape(nConvert::ToBullet(def.Size));
		
		btTransform transform;
		transform.setIdentity();
		mStartPosition = nConvert::ToBullet(def.Position);
		transform.setOrigin(mStartPosition);
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
		rbInfo.m_restitution = 0.25f;

		mBody = new btRigidBody(rbInfo);

		//mBody->setAngularFactor(0.f);
		//btVector3 factor = mBody->getLinearFactor();
		//factor[1] = 0;
		//factor[0] = 0;
	//	mBody->setLinearFactor(factor);
		
		mBody->setUserPointer(this);

		btTransform frameInB = btTransform::getIdentity();
		frameInB.setRotation(nConvert::ToBullet(glm::quat(glm::radians(glm::vec3(0,0,90)))));

		mSlider = new btSliderConstraint(*mBody, frameInB, false);
		mSlider->setLowerLinLimit(0);
		mSlider->setUpperLinLimit(5);
		
		
		mSpring = new btGeneric6DofSpringConstraint(*mBody, frameInB,true);
		mSpring->enableSpring(0,true);
		mSpring->setStiffness(0,100.f * def.Mass);
		mSpring->setLinearLowerLimit(btVector3(-6,0,0));
		mSpring->setLinearUpperLimit(btVector3(20, 0, 0));
		
		
	}
	void cLauncher::GetTransform(glm::mat4& transformOut)
	{
		btTransform transform;
		mBody->getMotionState()->getWorldTransform(transform);
		nConvert::ToGLM(transform, transformOut);
	}
	void cLauncher::ApplyForce(const glm::vec3& force)
	{
		mBody->activate(true);
		mBody->applyCentralForce(nConvert::ToBullet(force));
	}
	void cLauncher::ApplyImpulse(const glm::vec3& impulse)
	{
		mBody->activate(true);
		mBody->applyCentralImpulse(nConvert::ToBullet(impulse));
	}
	void cLauncher::Pull(const glm::vec3& direction)
	{
		mBody->activate(true);
		mSpring->enableSpring(0, false);
		btTransform transform = mBody->getWorldTransform();
		transform.setOrigin(mStartPosition + nConvert::ToBullet(glm::normalize(direction)*4.5f));
		
		mBody->setWorldTransform(transform);
	}
	void cLauncher::Release()
	{
		mBody->activate(true);
		mSpring->enableSpring(0, true);
	}
	void cLauncher::SetEntityId(int id)
	{
		mBody->setUserIndex(id);
	}
	int cLauncher::GetEntityId()
	{
		return mBody->getUserIndex();
	}
	void cLauncher::SetSecondaryId(int id)
	{
		mBody->setUserIndex2(id);
	}
	int cLauncher::GetSecondaryId()
	{
		return mBody->getUserIndex2();
	}
	void cLauncher::AddToWorld(btDynamicsWorld* world)
	{
		world->addRigidBody(mBody);
		world->addConstraint(mSlider);
		world->addConstraint(mSpring);
	}
	void cLauncher::RemoveFromWorld(btDynamicsWorld* world)
	{
		world->removeRigidBody(mBody);
		world->removeConstraint(mSlider);
		world->removeConstraint(mSpring);
	}
}
