#pragma once

#include <glm.hpp>
#include "physics/interfaces/iPaddleComponent.h"
#include "../iBulletComponent.h"

namespace DegenBulletPhysicsWrapper
{
	class cPaddle : public Degen::Physics::iPaddleComponent,
		public iBulletComponent
	{
	public:
		btRigidBody* mBody;
		btHingeConstraint* mConstraint;
		virtual ~cPaddle();
		cPaddle(Degen::Physics::sPaddleDef def);
		virtual void GetTransform(glm::mat4& transformOut) override;

		virtual void ApplyForce(const glm::vec3& force) override;
		void ApplyImpulse(const glm::vec3& impulse) override;
		void Hit(const glm::vec3& force) override;

		void SetEntityId(int id) override;
		int GetEntityId() override;
		void SetSecondaryId(int id) override;
		int GetSecondaryId() override;
		void AddToWorld(btDynamicsWorld* world) override;
		void RemoveFromWorld(btDynamicsWorld* world) override;
	};
}
