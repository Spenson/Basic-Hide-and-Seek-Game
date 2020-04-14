#pragma once
/**
 *	\file	cSphere.h
 *	\brief	Class header file
 *
 *	\desc	Contains the cSphere wrapper class definition
 */
#include <glm.hpp>
#include <physics/interfaces/iBallComponent.h>
#include "bullet/btBulletDynamicsCommon.h"

namespace DegenBulletPhysicsWrapper
{
	/**
	 * \class	cSphere
	 * \brief	Wrapper class
	 *
	 * \desc	A wrapper for a cRigidBody object containing a cSphere: iShape. Maintaining a pointer to the body to access and the shape for cleanup.
	 */
	
	class cSphere: public Degen::Physics::iBallComponent
	{
	public:
		btRigidBody* mBody;	// \desc	pointer to wrapped object to allow access
		
		/**
		 * \method		~cSphere
		 * \fullname	DegenMyPhysicsWrapper::cSphere::~cSphere
		 * \brief		dtor
		 *
		 * \access		virtual public 
		 *
		 * \desc		cleans up mBody and mShape
		 */
		virtual ~cSphere();

		/**
		 * \method		cSphere
		 * \fullname	DegenMyPhysicsWrapper::cSphere::cSphere
		 * \brief		ctor
		 *
		 * \access		public 
		 * \param		Degen::Physics::sSphereDef def
		 *
		 * \desc		creates Rigid body and sphere shape based off values in def
		 */
		cSphere(Degen::Physics::sBallDef def);
		
		/**
		 * \method		GetTransform
		 * \fullname	DegenMyPhysicsWrapper::cSphere::GetTransform
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
		 * \fullname	DegenMyPhysicsWrapper::cSphere::ApplyForce
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
	};
}
