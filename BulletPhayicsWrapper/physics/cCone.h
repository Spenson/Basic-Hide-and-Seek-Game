#pragma once

#include <glm.hpp>
#include "physics/interfaces/iConeComponent.h"
#include "bullet/btBulletDynamicsCommon.h"

namespace DegenBulletPhysicsWrapper
{
	class cCone : public Degen::Physics::iConeComponent
	{
	public:
		btRigidBody* mBody;
		virtual ~cCone();
		cCone(Degen::Physics::sConeDef def);
		virtual void GetTransform(glm::mat4& transformOut) override;

		virtual void ApplyForce(const glm::vec3& force) override;
		void ApplyImpulse(const glm::vec3& impulse) override;
	};
}
