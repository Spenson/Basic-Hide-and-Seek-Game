#pragma once

#include <glm.hpp>
#include "physics/interfaces/iBoxComponent.h"
#include "../iBulletComponent.h"

namespace DegenBulletPhysicsWrapper
{
	class cBox : public Degen::Physics::iBoxComponent,
		public iBulletComponent
	{
	public:
		btRigidBody* mBody;
		virtual ~cBox();
		cBox(Degen::Physics::sBoxDef def);
		virtual void GetTransform(glm::mat4& transformOut) override;

		virtual void ApplyForce(const glm::vec3& force) override;
		void ApplyImpulse(const glm::vec3& impulse) override;
		
		void SetEntityId(int id) override;
		int GetEntityId() override;
		void SetSecondaryId(int id) override;
		int GetSecondaryId() override;
		void AddToWorld(btDynamicsWorld* world) override;
		void RemoveFromWorld(btDynamicsWorld* world) override;
	};
}
