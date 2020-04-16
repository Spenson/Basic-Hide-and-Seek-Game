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
			cEntity() : user_id(0), unique_id(next_id++), name() {}

			template<class T> T* AddComponent();
			Component::iComponent* GetComponent(unsigned int type);
			std::vector<Component::iComponent*> GetComponents(unsigned int type);
			bool RemoveComponent(Component::iComponent* c);
			bool HasComponent(unsigned int type);

			// user set to identify object type or something use as you see fit
			unsigned int user_id;
			//generated id
			unsigned int unique_id;
			std::string name;

			std::vector<Component::iComponent*> components;
		private:
			static unsigned int next_id;
		};

		template<class T>
		inline T* cEntity::AddComponent()
		{
			T* c = new T();
			if (dynamic_cast<Component::iComponent*>(c) == nullptr)
			{
				return nullptr;
			}

			this->components.push_back(c);

			return c;
		}
	}
}
