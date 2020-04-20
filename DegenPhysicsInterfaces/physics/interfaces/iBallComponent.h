#pragma once
#include <glm.hpp>
#include "iPhysicsComponent.h"

namespace Degen
{

	namespace Physics
	{
		struct sBallDef
		{
			float Mass;
			float Radius;
			glm::vec3 Position;
			glm::quat Rotation;
		};

		class iBallComponent : public iPhysicsComponent
		{
		public:
			virtual ~iBallComponent() {}

			virtual void ApplyForce(const glm::vec3& force) = 0;
			virtual void ApplyImpulse(const glm::vec3& impulse) = 0;
			virtual void SetPosition(const glm::vec3& position) = 0;

		protected:
			iBallComponent() : iPhysicsComponent(eComponentType::ball) {}
		private:
			iBallComponent(const iBallComponent& other) = delete;
			iBallComponent& operator=(const iBallComponent& other) = delete;
		};
	}

}