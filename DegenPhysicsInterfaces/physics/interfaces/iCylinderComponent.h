#pragma once

#include <glm.hpp>
#include "iPhysicsComponent.h"

namespace Degen
{
	namespace Physics
	{
		struct sCylinderDef
		{
			float Mass;
			glm::vec3 Size;
			glm::vec3 Position;
			glm::quat Rotation;
		};

		class iCylinderComponent : public iPhysicsComponent
		{
		public:
			virtual ~iCylinderComponent() {}

			virtual void ApplyForce(const glm::vec3& force) = 0;
			virtual void ApplyImpulse(const glm::vec3& impulse) = 0;

		protected:
			iCylinderComponent() : iPhysicsComponent(eComponentType::cylinder) {}
		private:
			iCylinderComponent(const iCylinderComponent& other) = delete;
			iCylinderComponent& operator=(const iCylinderComponent& other) = delete;
		};
	}
}