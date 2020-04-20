#pragma once
#include "iPhysicsComponent.h"

namespace Degen
{
	namespace Physics
	{
		struct sTriggerRegionDef
		{
			glm::vec3 Position;
			glm::vec3 Size;
			glm::quat Rotation;
		};
		
		class iTriggerRegionComponent: public iPhysicsComponent
		{
		public:
			virtual ~iTriggerRegionComponent() {}

			virtual bool EntityIdIsTriggeredBy(int entityId) = 0;
			virtual bool SecondaryIdIsTriggeredBy(int entityId) = 0;

		protected:
			iTriggerRegionComponent(): iPhysicsComponent(eComponentType::triggerRegion) {}
		private:
			iTriggerRegionComponent(const iTriggerRegionComponent& other) = delete;
			iTriggerRegionComponent& operator=(const iTriggerRegionComponent& other) = delete;
		};
	}
}
