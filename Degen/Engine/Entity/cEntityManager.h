#pragma once
#include <vector>
#include "cEntity.h"


namespace Degen
{
	namespace Entity
	{
		class cEntityManager
		{
		public:
			cEntityManager();
			virtual ~cEntityManager();

			static cEntity* CreateEntity();
			
			static std::vector<cEntity*> entities;
		};
	}
}
