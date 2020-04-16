#pragma once

#include <glm.hpp>
#include "physics/interfaces/iLauncherComponent.h"
#include "bullet/btBulletDynamicsCommon.h"

namespace DegenBulletPhysicsWrapper
{
	class cLauncher : public Degen::Physics::iLauncherComponent
	{
	public:
		btRigidBody* mBody;
		//btSliderConstraint* mConstraint;
		btGeneric6DofSpringConstraint* mConstraint;
		virtual ~cLauncher();
		cLauncher(Degen::Physics::sLauncherDef def);
		virtual void GetTransform(glm::mat4& transformOut) override;

		virtual void ApplyForce(const glm::vec3& force) override;
		void ApplyImpulse(const glm::vec3& impulse) override;
	};
}
