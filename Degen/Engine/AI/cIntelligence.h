#pragma once
#include <string>
#include "../Entity/cEntity.h"
#include <glm.hpp>
#include "Graph.h"

namespace Degen {
	namespace Component {
		class Firefly;
	}
}

namespace Degen
{
	namespace AI
	{
		class cIntelligence
		{
		public:
			cIntelligence(std::string map_file);
			void Update(double dt);
			void WaitUpdate(double dt, Entity::cEntity* entity, Component::Firefly* gather);
			void ReturnUpdate(double dt, Entity::cEntity* entity, Component::Firefly* gather);
			void SearchUpdate(double dt, Entity::cEntity* entity, Component::Firefly* gather);
			void AddEntity(Entity::cEntity* entity);

			std::vector<Entity::cEntity*> entities;

			unsigned map_width, map_height;
			Node* home_node;
			Node* start_node;
			Graph* graph;


		};
	}
}
