#pragma once
#include "bullet/btBulletDynamicsCommon.h"

namespace DegenBulletPhysicsWrapper
{
	class iBulletComponent
	{
	public:
		virtual void AddToWorld(btDynamicsWorld* world) = 0;
		virtual void RemoveFromWorld(btDynamicsWorld* world) = 0;
	};
}