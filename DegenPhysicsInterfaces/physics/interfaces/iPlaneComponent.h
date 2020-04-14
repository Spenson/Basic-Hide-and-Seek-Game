#pragma once
#include <glm.hpp>
#include "iPhysicsComponent.h"

namespace Degen
{

namespace Physics
{
	struct sPlaneDef
	{
		float Constant;
		glm::vec3 Normal;
	};
	class iPlaneComponent : public iPhysicsComponent
	{
	public:
		virtual ~iPlaneComponent() {}

	protected:
		iPlaneComponent() : iPhysicsComponent(eComponentType::plane) {}
	private:
		iPlaneComponent(const iPlaneComponent& other) = delete;
		iPlaneComponent& operator=(const iPlaneComponent& other) = delete;
	};
}
}