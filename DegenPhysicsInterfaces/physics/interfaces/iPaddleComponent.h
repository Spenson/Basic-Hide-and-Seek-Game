#pragma once

#include <glm.hpp>
#include "iPhysicsComponent.h"

namespace Degen
{

	namespace Physics
	{
		struct sPaddleDef
		{
			float Mass;
			glm::vec3 Size;
			glm::vec3 Position;
			glm::quat Rotation;

			bool HingeOnPositiveX; 
		};

		class iPaddleComponent : public iPhysicsComponent
		{
		public:
			virtual ~iPaddleComponent() {}

			virtual void ApplyForce(const glm::vec3& force) = 0;
			virtual void Hit(const glm::vec3& force) = 0;
			virtual void ApplyImpulse(const glm::vec3& impulse) = 0;

		protected:
			iPaddleComponent() : iPhysicsComponent(eComponentType::paddle) {}
		private:
			iPaddleComponent(const iPaddleComponent& other) = delete;
			iPaddleComponent& operator=(const iPaddleComponent& other) = delete;
		};
	}

}