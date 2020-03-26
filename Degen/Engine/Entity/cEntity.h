#pragma once

#include <string>
#include <vector>

#include "../Component/iComponent.h"

namespace Degen
{
	namespace Entity
	{
		class cEntity
		{
		public:
			cEntity() : id(0), name() {}

			template<class T> T* AddComponent();
			Component::iComponent* GetComponent(unsigned int type);
			bool RemoveComponent(Component::iComponent* c);
			bool HasComponent(unsigned int type);

			unsigned int id;
			std::string name;

			std::vector<Component::iComponent*> components;
		};
	}
}
