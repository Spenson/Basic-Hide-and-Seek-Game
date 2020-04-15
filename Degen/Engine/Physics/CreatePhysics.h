#pragma once
#include "../FileReading/JsonHelpers.h"
#include <physics/interfaces/physics_interfaces.h>


namespace Degen
{
	namespace Physics
	{
		iPhysicsFactory* CreatePhysicsFactory();
		void DestroyPhysicsFactory(iPhysicsFactory* factory);

		iBallComponent* CreateBallPhysicsComponent(const Json::Value& jsonBall);
		iPlaneComponent* CreatePlanePhysicsComponent(const Json::Value& jsonPlane);
		iBoxComponent* CreateBoxPhysicsComponent(const Json::Value& jsonBox);
	}
}
