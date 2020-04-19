#pragma once
#include "iBallComponent.h"
#include "iBoxComponent.h"
#include "iPlaneComponent.h"
#include "iConeComponent.h"
#include "iCylinderComponent.h"
#include "iPaddleComponent.h"
#include "iPhysicsWorld.h"
#include "iLauncherComponent.h"
#include "iTriggerRegionComponent.h"

namespace Degen
{

namespace Physics
{
	class iPhysicsFactory
	{
	public:
		virtual ~iPhysicsFactory() {}
		virtual iPhysicsWorld* CreateWorld() = 0;
		// components
		virtual iBallComponent* CreateBall(const sBallDef& def) = 0;
		virtual iBoxComponent* CreateBox(const sBoxDef& def) = 0;
		virtual iPlaneComponent* CreatePlane(const sPlaneDef& def) = 0;
		virtual iConeComponent* CreateCone(const sConeDef& def) = 0;
		virtual iCylinderComponent* CreateCylinder(const sCylinderDef& def) = 0;
		virtual iPaddleComponent* CreatePaddle(const sPaddleDef& def) = 0;
		virtual iLauncherComponent* CreateLauncher(const sLauncherDef& def) = 0;
		virtual iTriggerRegionComponent* CreateTriggerRegion(const sTriggerRegionDef& def) = 0;
	};
}
}
