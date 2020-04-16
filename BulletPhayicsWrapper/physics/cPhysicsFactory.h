#pragma once
/**
 *	\file	cPhysicsFactory.h
 *	\brief	Class header file
 *
 *	\desc	Contains the cPhysicsFactory class definition
 */

#include <physics/interfaces/iPhysicsFactory.h>

namespace DegenBulletPhysicsWrapper
{
	/**
	 *	\class	cPhysicsFactory
	 *	\brief	Factory class
	 *
	 *	\desc	A factory class for creating physics objects
	 */
	class cPhysicsFactory: public Degen::Physics::iPhysicsFactory
	{
	public:
		
		/**
		 * \method    ~cPhysicsFactory
		 * \fullname  DegenMyPhysicsWrapper::cPhysicsFactory::~cPhysicsFactory
		 * \brief     dtor
		 *
		 * \access    virtual public 
		 *
		 * \desc      default dtor
		 */
		virtual ~cPhysicsFactory() = default;
		
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
		Degen::Physics::iPhysicsWorld* CreateWorld() override;
		
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
		Degen::Physics::iBallComponent* CreateBall(const Degen::Physics::sBallDef& def) override;

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
		Degen::Physics::iPlaneComponent* CreatePlane(const Degen::Physics::sPlaneDef& def) override;


		Degen::Physics::iBoxComponent* CreateBox(const Degen::Physics::sBoxDef& def) override;
		Degen::Physics::iConeComponent* CreateCone(const Degen::Physics::sConeDef& def) override;
		Degen::Physics::iCylinderComponent* CreateCylinder(const Degen::Physics::sCylinderDef& def) override;
		Degen::Physics::iPaddleComponent* CreatePaddle(const Degen::Physics::sPaddleDef& def) override;
		Degen::Physics::iLauncherComponent* CreateLauncher(const Degen::Physics::sLauncherDef& def) override;
	};
}
