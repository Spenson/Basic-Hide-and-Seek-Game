#include "cEntity.h"

namespace Degen
{
	namespace Entity
	{
		unsigned int cEntity::next_id = 0;
		
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
		std::vector<Component::iComponent*> cEntity::GetComponents(unsigned int type)
		{
			std::vector<Component::iComponent*> comps;
			for (unsigned int i = 0; i < components.size(); i++)
			{
				if (type == components.at(i)->Type())
				{
					comps.push_back(components.at(i));
				}
			}
			//Entity does not contain specified component.
			return comps;
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