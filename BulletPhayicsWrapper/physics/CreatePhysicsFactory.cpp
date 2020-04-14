/**
 * \file	CreatePhysicsFactory.cpp
 * \path	D:\GDP\Degenerate\DegenMyPhysicsWrapper\physics
 * \brief	Function Implementation File
 *
 * \desc	contains the implementation of CreatePhysicsFactory
 */
#include "CreatePhysicsFactory.h"

#include "cPhysicsFactory.h"

 /**
  * \method		CreatePhysicsFactory
  * \fullname	CreatePhysicsFactory
  * \brief		Create Factory Method
  *
  * \access		DLL_EXPORT global
  * \returns		Degen::Physics*
  *
  * \desc		Creates a DegenMyPhysicsWrapper Factory and returns a pointer
  *              Exported for dll access
  */
DLL_EXPORT Degen::Physics::iPhysicsFactory* CreatePhysicsFactory()
{
	return new DegenBulletPhysicsWrapper::cPhysicsFactory();
}
