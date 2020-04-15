#pragma once

#include <glm.hpp>
#include "physics/interfaces/iBoxComponent.h"
#include "bullet/btBulletDynamicsCommon.h"

namespace DegenBulletPhysicsWrapper
{
	class cBox : public Degen::Physics::iBoxComponent
	{
	public:
		btRigidBody* mBody;
		virtual ~cBox();
		cBox(Degen::Physics::sBoxDef def);
		virtual void GetTransform(glm::mat4& transformOut) override;

		virtual void ApplyForce(const glm::vec3& force) override;
		void ApplyImpulse(const glm::vec3& impulse) override;
	};
}
