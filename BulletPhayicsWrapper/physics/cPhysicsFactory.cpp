/**
 * \file	cPhysicsFactory.cpp
 * \path	D:\GDP\Degenerate\DegenMyPhysicsWrapper\physics
 * \brief	Class Implementation File
 *
 * \desc	contains the implementation of the cPhysicsFactory class
 */

#include "cPhysicsFactory.h"
#include "cPhysicsWorld.h"
#include "cSphere.h"
#include "cPlane.h"
#include "cBox.h"

namespace DegenBulletPhysicsWrapper
{
	/**
	 * \method    CreateWorld
	 * \fullname  DegenMyPhysicsWrapper::cPhysicsFactory::CreateWorld
	 * \brief     Factory create method
	 *
	 * \access    public
	 * \returns   Degen::Physics::iPhysicsWorld*
	 *
	 * \desc      Override iPhysicsFactory method. Creates physics world and returns ptr
	 */
	Degen::Physics::iPhysicsWorld* cPhysicsFactory::CreateWorld()
	{
		return new cPhysicsWorld();
	}

	/**
	 * \method    CreateSphere
	 * \fullname  DegenMyPhysicsWrapper::cPhysicsFactory::CreateSphere
	 * \brief     Factory create method
	 *
	 * \access    public
	 * \returns   Degen::Physics::iBallComponent*
	 * \param     const Degen::Physics::sSphereDef & def
	 *
	 * \desc      Override iPhysicsFactory method. Creates sphere based off def and returns ptr
	 */
	Degen::Physics::iBallComponent* cPhysicsFactory::CreateBall(const Degen::Physics::sBallDef& def)
	{
		return new cSphere(def);
	}

	/**
	 * \method    CreatePlane
	 * \fullname  DegenMyPhysicsWrapper::cPhysicsFactory::CreatePlane
	 * \brief     Factory create method
	 *
	 * \access    public
	 * \returns   Degen::Physics::iPlaneComponent*
	 * \param     const Degen::Physics::sPlaneDef & def
	 *
	 * \desc      Override iPhysicsFactory method. Creates plane based off def and returns ptr
	 */
	Degen::Physics::iPlaneComponent* cPhysicsFactory::CreatePlane(const Degen::Physics::sPlaneDef& def)
	{
		return new cPlane(def);
	}

	Degen::Physics::iBoxComponent* cPhysicsFactory::CreateBox(const Degen::Physics::sBoxDef& def)
	{
		return new cBox(def);
	}

}
