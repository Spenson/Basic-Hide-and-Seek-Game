#pragma once

/**
 * \file	cPhysicsWorld.h
 * \path	D:\GDP\Degenerate\DegenMyPhysicsWrapper\physics
 * \brief	Class header file
 *
 * \desc	Contains the cPhysicsWorld class definition
 */

#include <physics/interfaces/iPhysicsWorld.h>
#include "bullet/btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

namespace DegenBulletPhysicsWrapper
{
	/**
	 * \class	cPhysicsWorld
	 * \brief	Wrapper class
	 *
	 * \desc	A wrapper for a cWorld object. Maintaining a pointer to the world to access
	 */
	class cPhysicsWorld : public Degen::Physics::iPhysicsWorld
	{
		Degen::Physics::iCollisionListener* mCollisionListener;
		
		btDynamicsWorld* mWorld;

		btDefaultCollisionConfiguration* mCollisionConfiguration;
		btCollisionDispatcher* mDispatcher;
		btBroadphaseInterface* mOverlappingPairCache;
		btGhostPairCallback* mGhostPairCallback;
		btSequentialImpulseConstraintSolver* mSolver;
	public:
		/**
		 * \method		~cPhysicsWorld
		 * \fullname	DegenMyPhysicsWrapper::cPhysicsWorld::~cPhysicsWorld
		 * \brief		dtor
		 *
		 * \access		virtual public 
		 *
		 * \desc		cleans up mWorld
		 */
		virtual ~cPhysicsWorld();

		/**
		 * \method		cPhysicsWorld
		 * \fullname	DegenMyPhysicsWrapper::cPhysicsWorld::cPhysicsWorld
		 * \brief		ctor
		 *
		 * \access		public
		 * 
		 * \desc		initialize mWorld
		 */
		cPhysicsWorld();

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
		void Update(float dt) override;
		
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
		bool AddComponent(Degen::Physics::iPhysicsComponent* component) override;
		
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
		bool RemoveComponent(Degen::Physics::iPhysicsComponent* component) override;
		
		void SetCollisionListener(Degen::Physics::iCollisionListener* listener) override;

		void SetGravity(glm::vec3 gravity) override;
	};
}
