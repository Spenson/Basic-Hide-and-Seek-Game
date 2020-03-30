#pragma once
#include <string>
#include "../Entity/cEntity.h"
#include <glm.hpp>

namespace Degen
{
	namespace AI
	{
		class cIntelligence
		{
		public:
			struct node;

			enum node_type
			{
				open,
				difficult,
				wall,
				start,
				resource,
				home
			};

			struct edge
			{
				node* a;
				node* b;
				unsigned length;
			};

			struct node
			{
				node_type type;
				glm::vec3 world_position;
				unsigned x, y; // for refrence
				std::vector<edge*> edges;
			};


			cIntelligence(std::string map_file);
			void Update(double dt);
			void AddEntity(Entity::cEntity* entity);

			std::vector<Entity::cEntity*> entities;

			unsigned map_width, map_height;
			std::vector<node*> map;


		};
	}
}
