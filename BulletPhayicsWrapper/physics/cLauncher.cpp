#include "cLauncher.h"
#include "nConvert.h"

namespace DegenBulletPhysicsWrapper
{

	cLauncher::~cLauncher()
	{
		delete mConstraint;
		mConstraint = 0;
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

		btTransform frameInB = btTransform::getIdentity();

	/*	mConstraint = new btSliderConstraint(*mBody, frameInB, false);*/
		mConstraint = new btGeneric6DofSpringConstraint(*mBody, frameInB,true);
		mConstraint->enableSpring(1,true);
		mConstraint->setStiffness(1,20.f * def.Mass);
		mConstraint->setLinearLowerLimit(btVector3(0,-1,0));
		mConstraint->setLinearUpperLimit(btVector3(0, -5, 0));
		mConstraint->setAngularLowerLimit(btVector3(0,0,0));
		mConstraint->setAngularUpperLimit(btVector3(0,0,0));
		
		
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
}
