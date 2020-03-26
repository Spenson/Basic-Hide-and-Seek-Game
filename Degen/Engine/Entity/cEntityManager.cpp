#include "cEntityManager.h"
namespace Degen
{
	namespace Entity
	{
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
	}
}