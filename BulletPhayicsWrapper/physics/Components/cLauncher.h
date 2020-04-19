#pragma once

#include <glm.hpp>
#include "physics/interfaces/iLauncherComponent.h"
#include "../iBulletComponent.h"

namespace DegenBulletPhysicsWrapper
{
	class cLauncher : public Degen::Physics::iLauncherComponent,
		public iBulletComponent
	{
	public:
		btRigidBody* mBody;
		btSliderConstraint* mSlider;
		btGeneric6DofSpringConstraint* mSpring;
		btVector3 mStartPosition;
		virtual ~cLauncher();
		cLauncher(Degen::Physics::sLauncherDef def);
		virtual void GetTransform(glm::mat4& transformOut) override;

		virtual void ApplyForce(const glm::vec3& force) override;
		void ApplyImpulse(const glm::vec3& impulse) override;
		void Pull(const glm::vec3& direction) override;
		void Release() override;

		void SetEntityId(int id) override;
		int GetEntityId() override;
		void SetSecondaryId(int id) override;
		int GetSecondaryId() override;
		void AddToWorld(btDynamicsWorld* world) override;
		void RemoveFromWorld(btDynamicsWorld* world) override;
	};
}
