#include "cEntityManager.h"
namespace Degen
{
	namespace Entity
	{
		std::vector<cEntity*> cEntityManager::entities;
		
		cEntityManager::cEntityManager() = default;

		cEntityManager::~cEntityManager()
		{
			for(auto ent:entities)
			{
				delete ent;
			}
			entities.clear();
		}
		cEntity* cEntityManager::CreateEntity()
		{
			cEntity* ent = new cEntity();
			entities.push_back(ent);
			return ent;
		}
		cEntity* cEntityManager::GetEntity(unsigned id)
		{
			for (auto* entity : entities)
			{
				if(entity->id == id) return entity;
			}
			return nullptr;
		}
		cEntity* cEntityManager::GetEntity(std::string name)
		{
			for (auto* entity : entities)
			{
				if (entity->name == name) return entity;
			}
			return nullptr;
		}
	}
}