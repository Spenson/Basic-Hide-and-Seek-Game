#pragma once

#include <glm.hpp>
#include <utility>
#include <vector>
#include "../Entity/cEntity.h"

namespace Degen
{
	namespace AI
	{
		
		struct Node
		{
			enum node_type
			{
				open,
				difficult,
				wall,
				start,
				resource,
				home
			};

			node_type type;
			glm::vec3 world_position;
			std::vector<std::pair< Node*, float>> children;

			struct Node* parent;
			bool visited;
			float gCostSoFar;
			float hDistance;

			Entity::cEntity* entity;
		};
		
		class Graph
		{
		public:
			Node* CreateNode(Node::node_type type, glm::vec3 pos);
			void ResetGraph();
			void GernerateSquareMapEdges(unsigned height, unsigned width, float short_path, float long_path);
			void AddEdge(Node* parent, Node* child, float weight, bool bUndirected = true);
			std::vector<Node*> nodes;
		};
	}
}
