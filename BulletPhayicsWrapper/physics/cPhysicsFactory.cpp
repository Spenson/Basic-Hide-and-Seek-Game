/**
 * \file	cPhysicsFactory.cpp
 * \path	D:\GDP\Degenerate\DegenMyPhysicsWrapper\physics
 * \brief	Class Implementation File
 *
 * \desc	contains the implementation of the cPhysicsFactory class
 */

#include "cPhysicsFactory.h"
#include "cPhysicsWorld.h"
#include "cBall.h"
#include "cPlane.h"
#include "cBox.h"
#include "cCone.h"
#include "cCylinder.h"
#include "cPaddle.h"
#include "cLauncher.h"

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
		return new cBall(def);
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

	Degen::Physics::iConeComponent* cPhysicsFactory::CreateCone(const Degen::Physics::sConeDef& def)
	{
		return new cCone(def);
	}

	Degen::Physics::iCylinderComponent* cPhysicsFactory::CreateCylinder(const Degen::Physics::sCylinderDef& def)
	{
		return new cCylinder(def);
	}

	Degen::Physics::iPaddleComponent* cPhysicsFactory::CreatePaddle(const Degen::Physics::sPaddleDef& def)
	{
		return new cPaddle(def);
	}

	Degen::Physics::iLauncherComponent* cPhysicsFactory::CreateLauncher(const Degen::Physics::sLauncherDef& def)
	{
		return new cLauncher(def);
	}

}
