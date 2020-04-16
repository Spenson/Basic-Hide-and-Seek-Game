#pragma once

#include <glm.hpp>
#include "iPhysicsComponent.h"

namespace Degen
{

	namespace Physics
	{
		struct sLauncherDef
		{
			float Mass;
			glm::vec3 Size;
			glm::vec3 Position;
			glm::quat Rotation;

			
		};

		class iLauncherComponent : public iPhysicsComponent
		{
		public:
			virtual ~iLauncherComponent() {}

			virtual void ApplyForce(const glm::vec3& force) = 0;
			virtual void ApplyImpulse(const glm::vec3& impulse) = 0;
			virtual void Pull(const glm::vec3& direction) = 0;
			virtual void Release() = 0;
			

		protected:
			iLauncherComponent() : iPhysicsComponent(eComponentType::launcher) {}
		private:
			iLauncherComponent(const iLauncherComponent& other) = delete;
			iLauncherComponent& operator=(const iLauncherComponent& other) = delete;
		};
	}

}