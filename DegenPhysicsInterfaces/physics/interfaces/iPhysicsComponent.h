#pragma once
#include <glm.hpp>
#include "eComponentType.h"

namespace Degen
{

namespace Physics
{
	class iPhysicsComponent
	{
	public:
		virtual ~iPhysicsComponent() {}
		inline const eComponentType& GetComponentType() { return mComponentType; }

		virtual void GetTransform(glm::mat4& transformOut) = 0;

		virtual void SetEntityId(int id) = 0;
		virtual int GetEntityId() = 0;
		virtual void SetSecondaryId(int id) = 0;
		virtual int GetSecondaryId() = 0;
		
	protected:
		iPhysicsComponent(eComponentType componentType)
			: mComponentType(componentType) {}
	private:
		eComponentType mComponentType;

		// get rid of these!
		iPhysicsComponent() = delete;
		iPhysicsComponent(const iPhysicsComponent& other) = delete;
		iPhysicsComponent& operator=(const iPhysicsComponent& other) = delete;
	};
}
}