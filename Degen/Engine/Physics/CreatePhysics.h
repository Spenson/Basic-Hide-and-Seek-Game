#pragma once
#include "../FileReading/JsonHelpers.h"
#include <physics/interfaces/physics_interfaces.h>


namespace Degen
{
	namespace Physics
	{
		iPhysicsFactory* CreatePhysicsFactory();
		void DestroyPhysicsFactory(iPhysicsFactory* factory);

		iPhysicsComponent* CreatePhysicsComponent(const Json::Value& json);
		
		iBallComponent* CreateBallPhysicsComponent(const Json::Value& jsonBall);
		iPlaneComponent* CreatePlanePhysicsComponent(const Json::Value& jsonPlane);
		iBoxComponent* CreateBoxPhysicsComponent(const Json::Value& jsonBox);
		iCylinderComponent* CreateCylinderPhysicsComponent(const Json::Value& jsonCylinder);
		iConeComponent* CreateConePhysicsComponent(const Json::Value& jsonCone);
		iPaddleComponent* CreatePaddlePhysicsComponent(const Json::Value& jsonPaddle);
		iLauncherComponent* CreateLauncherPhysicsComponent(const Json::Value& jsonLauncher);
		iTriggerRegionComponent* CreateTriggerRegionPhysicsComponent(const Json::Value& jsonTriggerRegion);
	}
}
