#include "cEntity.h"

namespace Degen
{
	namespace Entity
	{
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

		Component::iComponent* cEntity::GetComponent(unsigned int type)
		{
			for (unsigned int i = 0; i < components.size(); i++)
			{
				if (type == components.at(i)->Type())
				{
					return components.at(i);
				}
			}
			//Entity does not contain specified component.
			return nullptr;
		}
		bool cEntity::RemoveComponent(Component::iComponent* c)
		{
			for (unsigned int i = 0; i < components.size(); i++)
			{
				if (components.at(i) == c)
				{
					delete components.at(i);
					//components.at(i) = nullptr; // redundant with next line
					components.erase(components.begin() + i);
					return true;
				}
			}
			// component not found
			return false;
		}
		bool cEntity::HasComponent(unsigned int type)
		{
			for (unsigned int i = 0; i < components.size(); i++)
			{
				if (type == components.at(i)->Type())
				{
					return true;
				}
			}
			return false;
		}
	}
}