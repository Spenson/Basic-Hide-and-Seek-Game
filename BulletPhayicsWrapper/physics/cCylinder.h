#pragma once

#include <glm.hpp>
#include "physics/interfaces/iCylinderComponent.h"
#include "bullet/btBulletDynamicsCommon.h"

namespace DegenBulletPhysicsWrapper
{
	class cCylinder : public Degen::Physics::iCylinderComponent
	{
	public:
		btRigidBody* mBody;
		virtual ~cCylinder();
		cCylinder(Degen::Physics::sCylinderDef def);
		virtual void GetTransform(glm::mat4& transformOut) override;

		virtual void ApplyForce(const glm::vec3& force) override;
		void ApplyImpulse(const glm::vec3& impulse) override;
	};
}
