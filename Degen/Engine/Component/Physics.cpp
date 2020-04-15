#include "Physics.h"
#include "../FileReading/JsonHelpers.h"
#include "../Physics/CreatePhysics.h"



namespace Degen
{
	namespace Component
	{
		Physics::Physics() : iComponent(PHYSICS_COMPONENT),
			comp(nullptr)
		{
		}

		bool Degen::Component::Physics::Deserialize(Json::Value& json)
		{
			std::string type;
			JsonHelp::Set(json["type"], type);
			if (type == "ball")
			{
				comp = Degen::Physics::CreateBallPhysicsComponent(json);
			}
			if (type == "plane")
			{
				comp = Degen::Physics::CreatePlanePhysicsComponent(json);
			}
			if (type == "box")
			{
				comp = Degen::Physics::CreateBoxPhysicsComponent(json);
			}

			return true;
		}
	}
}