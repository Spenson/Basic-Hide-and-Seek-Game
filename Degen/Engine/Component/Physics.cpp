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

		bool Physics::Deserialize(Json::Value& json)
		{
			comp = Degen::Physics::CreatePhysicsComponent(json);

			return true;
		}
	}
}