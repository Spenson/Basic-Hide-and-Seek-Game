#pragma once
/**
 *	\file	cBall.h
 *	\brief	Class header file
 *
 *	\desc	Contains the cBall wrapper class definition
 */
#include <glm.hpp>
#include <physics/interfaces/iBallComponent.h>
#include "../iBulletComponent.h"

namespace DegenBulletPhysicsWrapper
{
	/**
	 * \class	cBall
	 * \brief	Wrapper class
	 *
	 * \desc	A wrapper for a cRigidBody object containing a cBall: iShape. Maintaining a pointer to the body to access and the shape for cleanup.
	 */
	
	class cBall:
	public Degen::Physics::iBallComponent,
	public iBulletComponent
	{
	public:
		btRigidBody* mBody;	// \desc	pointer to wrapped object to allow access
		
		/**
		 * \method		~cBall
		 * \fullname	DegenMyPhysicsWrapper::cBall::~cBall
		 * \brief		dtor
		 *
		 * \access		virtual public 
		 *
		 * \desc		cleans up mBody and mShape
		 */
		virtual ~cBall();

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
		cBall(Degen::Physics::sBallDef def);
		
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
		virtual void GetTransform(glm::mat4& transformOut) override;
		
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
		virtual void ApplyForce(const glm::vec3& force) override;
		void ApplyImpulse(const glm::vec3& impulse) override;
		
		void SetEntityId(int id) override;
		int GetEntityId() override;
		void SetSecondaryId(int id) override;
		int GetSecondaryId() override;
		void AddToWorld(btDynamicsWorld* world) override;
		void RemoveFromWorld(btDynamicsWorld* world) override;
	};
}
