#pragma once
/**
 * \file	CreatePhysicsFactory.h
 * \path	D:\GDP\Degenerate\DegenMyPhysicsWrapper\physics
 * \brief	Function Header File
 *
 * \desc	contains the signature of CreatePhysicsFactory
 */
#include <physics/interfaces/iPhysicsFactory.h>

#define DLL_EXPORT extern "C" __declspec(dllexport)

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
DLL_EXPORT Degen::Physics::iPhysicsFactory* CreatePhysicsFactory();
