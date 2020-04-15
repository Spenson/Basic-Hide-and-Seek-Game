#pragma once

#include <glm.hpp>
#include "iPhysicsComponent.h"

namespace Degen
{
	namespace Physics
	{
		struct sConeDef
		{
			float Mass;
			float Height;
			float Radius;
			glm::vec3 Position;
			glm::quat Rotation;
		};

		class iConeComponent : public iPhysicsComponent
		{
		public:
			virtual ~iConeComponent() {}

			virtual void ApplyForce(const glm::vec3& force) = 0;
			virtual void ApplyImpulse(const glm::vec3& impulse) = 0;

		protected:
			iConeComponent() : iPhysicsComponent(eComponentType::cone) {}
		private:
			iConeComponent(const iConeComponent& other) = delete;
			iConeComponent& operator=(const iConeComponent& other) = delete;
		};
	}
}