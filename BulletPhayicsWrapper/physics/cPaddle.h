#pragma once

#include <glm.hpp>
#include "physics/interfaces/iPaddleComponent.h"
#include "bullet/btBulletDynamicsCommon.h"

namespace DegenBulletPhysicsWrapper
{
	class cPaddle : public Degen::Physics::iPaddleComponent
	{
	public:
		btRigidBody* mBody;
		btHingeConstraint* mConstraint;
		virtual ~cPaddle();
		cPaddle(Degen::Physics::sPaddleDef def);
		virtual void GetTransform(glm::mat4& transformOut) override;

		virtual void ApplyForce(const glm::vec3& force) override;
		void ApplyImpulse(const glm::vec3& impulse) override;
	};
}
