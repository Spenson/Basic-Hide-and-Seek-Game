/**
 * \file	cPhysicsWorld.cpp
 * \path	D:\GDP\Degenerate\DegenMyPhysicsWrapper\physics
 * \brief	Class Implementation File
 *
 * \desc	contains the implementation of the cPhysicsWorld class
 */

#include "cPhysicsWorld.h"
#include "cPlane.h"
#include "cSphere.h"
#include "nConvert.h"
#include "cBox.h"

namespace DegenBulletPhysicsWrapper
{
	/**
	 * \method		~cPhysicsWorld
	 * \fullname	DegenMyPhysicsWrapper::cPhysicsWorld::~cPhysicsWorld
	 * \brief		dtor
	 *
	 * \access		virtual public
	 *
	 * \desc		cleans up mWorld
	 */
	cPhysicsWorld::~cPhysicsWorld()
	{
		// Bullet Order			

		//delete dynamics world
		delete mWorld;

		//delete solver
		delete mSolver;

		//delete broadphase
		delete mOverlappingPairCache;

		//delete dispatcher
		delete mDispatcher;

		delete mCollisionConfiguration;

	}
	/**
		 * \method		cPhysicsWorld
		 * \fullname	DegenMyPhysicsWrapper::cPhysicsWorld::cPhysicsWorld
		 * \brief		ctor
		 *
		 * \access		public
		 *
		 * \desc		initialize mWorld, mCollisionConfiguration, mDispatcher, mOverlappingPairCache, mSolver
		 */
	cPhysicsWorld::cPhysicsWorld()
	{
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		mCollisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		mOverlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		mSolver = new btSequentialImpulseConstraintSolver;

		mWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration);

		mWorld->setGravity(btVector3(0, -5, 0));
	}

	/**
	 * \method		Update
	 * \fullname	DegenMyPhysicsWrapper::cPhysicsWorld::Update
	 * \brief		cWorld Update wrapper
	 *
	 * \access		public
	 * \returns		void
	 * \param		float dt : delta-time/timestep between updates
	 *
	 * \desc		calls mWorld's Update method passing dt
	 */
	void cPhysicsWorld::Update(float dt)
	{
		mWorld->stepSimulation(dt, 10);
		/*// todo collision stuff
		if (mCollisionListener)
		{

		}
		//*/
	}

	/**
	 * \method		AddObject
	 * \fullname	DegenMyPhysicsWrapper::cPhysicsWorld::AddObject
	 * \brief		cWorld AddRigidBody wrapper
	 *
	 * \access		public
	 * \returns		void
	 * \param		Degen::Physics::iPhysicsComponent * component : physics object to add to mWorld
	 *
	 * \desc		Adds object to the physics world
	 */
	bool cPhysicsWorld::AddComponent(Degen::Physics::iPhysicsComponent* component)
	{
		if (!component)
		{
			return false;
		}


		switch (component->GetComponentType())
		{
			case Degen::Physics::eComponentType::plane:
				mWorld->addRigidBody(dynamic_cast<cPlane*>(component)->mBody);
				break;
			case Degen::Physics::eComponentType::ball:
				mWorld->addRigidBody(dynamic_cast<cSphere*>(component)->mBody);
				break;
			case Degen::Physics::eComponentType::box:
				mWorld->addRigidBody(dynamic_cast<cBox*>(component)->mBody);
				break;
		}
		return true;
	}

	/**
	 * \method		RemoveObject
	 * \fullname	DegenMyPhysicsWrapper::cPhysicsWorld::RemoveObject
	 * \brief		cWorld RemoveRigidBody wrapper
	 *
	 * \access		public
	 * \returns		void
	 * \param		Degen::Physics::iPhysicsComponent * component
	 *
	 * \desc		removes object from the physics world
	 */
	bool cPhysicsWorld::RemoveComponent(Degen::Physics::iPhysicsComponent* component)
	{
		if (!component)
		{
			return false;
		}


		switch (component->GetComponentType())
		{
			case Degen::Physics::eComponentType::plane:
				mWorld->removeRigidBody(dynamic_cast<cPlane*>(component)->mBody);
				break;
			case Degen::Physics::eComponentType::ball:
				mWorld->removeRigidBody(dynamic_cast<cSphere*>(component)->mBody);
				break;
		}
		return true;
	}

	void cPhysicsWorld::SetCollisionListener(Degen::Physics::iCollisionListener* listener)
	{
	}

	void cPhysicsWorld::SetGravity(glm::vec3 gravity)
	{
		mWorld->setGravity(nConvert::ToBullet(gravity));
	}

}
