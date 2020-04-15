#pragma once

#include <glm.hpp>
#include "iPhysicsComponent.h"

namespace Degen
{

	namespace Physics
	{
		struct sBoxDef
		{
			float Mass;
			glm::vec3 Size;
			glm::vec3 Position;
		};

		class iBoxComponent : public iPhysicsComponent
		{
		public:
			virtual ~iBoxComponent() {}

			virtual void ApplyForce(const glm::vec3& force) = 0;
			virtual void ApplyImpulse(const glm::vec3& impulse) = 0;

		protected:
			iBoxComponent() : iPhysicsComponent(eComponentType::box) {}
		private:
			iBoxComponent(const iBoxComponent& other) = delete;
			iBoxComponent& operator=(const iBoxComponent& other) = delete;
		};
	}

}