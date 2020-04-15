#include <Windows.h>
#include <iostream>
#include "CreatePhysics.h"
#include "../Globals.h"

namespace Degen
{
	namespace Physics
	{

		typedef iPhysicsFactory* (*f_CreatePhysicsFactory)();
		HINSTANCE hGetPhysicsProckDll = 0;
		iPhysicsFactory* Degen::Physics::CreatePhysicsFactory()
		{
			iPhysicsFactory* factory;
			
			const char* physicsDLLName = "BulletPhysicsWrapper.dll";
			//const char* physicsDLLName = "BulletPhysicsWrapper.dll";
			hGetPhysicsProckDll = LoadLibraryA(physicsDLLName);
			if (!hGetPhysicsProckDll)
			{
				std::cout << "InitPhysics() failed. Could not load " << physicsDLLName << std::endl;
				return nullptr;
			}
			f_CreatePhysicsFactory CreatePhysicsFactory = (f_CreatePhysicsFactory)GetProcAddress(hGetPhysicsProckDll, "CreatePhysicsFactory");
			if (!CreatePhysicsFactory)
			{
				FreeLibrary(hGetPhysicsProckDll);
				hGetPhysicsProckDll = 0;
				std::cout << "InitPhysics() failed. Could not get the CreatePhysicsFactory function pointer." << std::endl;
				return nullptr;
			}
			factory = CreatePhysicsFactory();
			if (!factory)
			{
				FreeLibrary(hGetPhysicsProckDll);
				hGetPhysicsProckDll = 0;
				std::cout << "InitPhysics() failed. CreatePhysicsFactory() call failed." << std::endl;
				return nullptr;
			}
			//gPhysicsWorld = gPhysicsFactory->CreateWorld();
			return factory;
		}


		void DestroyPhysicsFactory(iPhysicsFactory* factory)
		{
			if (factory)
			{
				delete factory;
				factory = 0;
			}
			if (hGetPhysicsProckDll)
			{
				FreeLibrary(hGetPhysicsProckDll);
				hGetPhysicsProckDll = 0;
			}
		}
		
		iBallComponent* CreateBallPhysicsComponent(const Json::Value& jsonBall)
		{
			if (!jsonBall.isObject()) return 0;

			sBallDef def;
			if (JsonHelp::Set(jsonBall["position"], def.Position)
				&& JsonHelp::Set(jsonBall["mass"], def.Mass)
				&& JsonHelp::Set(jsonBall["radius"], def.Radius))
			{
				return PhysicsFactory->CreateBall(def);
			}
			return 0;
		}
		iPlaneComponent* CreatePlanePhysicsComponent(const Json::Value& jsonPlane)
		{
			if (!jsonPlane.isObject()) return 0;
			sPlaneDef def;
			float constant;
			if (JsonHelp::Set(jsonPlane["constant"], constant)
				&& JsonHelp::Set(jsonPlane["normal"], def.Normal))
			{
				def.Normal = glm::normalize(def.Normal);
				//Just go strait to constant becuase it makes more sense to me
				// for a plane with equation:
				// D = Ax + Bx + Cx
				// D:= constant
				// <A,B,C>:=normal
				// <x,y,z>:=any point on the plane
				// D = normal dot point on plane
				def.Constant = constant;// glm::dot(def.Normal, position);
				return PhysicsFactory->CreatePlane(def);
			}
			return 0;
		}
		iBoxComponent* CreateBoxPhysicsComponent(const Json::Value& jsonBox)
		{
			if (!jsonBox.isObject()) return 0;

			sBoxDef def;
			if (JsonHelp::Set(jsonBox["position"], def.Position)
				&& JsonHelp::Set(jsonBox["mass"], def.Mass)
				&& JsonHelp::Set(jsonBox["size"], def.Size))
			{
				return PhysicsFactory->CreateBox(def);
			}
			return 0;
		}
	}
}
