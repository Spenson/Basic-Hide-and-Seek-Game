#include "Graph.h"
#include "../Component/Render.h"

namespace Degen
{
	namespace AI
	{

		Node* Graph::CreateNode(Node::node_type type, glm::vec3 pos)
		{
			Node* node = new Node;
			node->type = type;
			node->world_position = pos;

			node->parent = nullptr;
			node->visited = false;
			node->gCostSoFar = FLT_MAX;
			node->hDistance = 0.f;

			nodes.push_back(node);

			return node;
		}
		void Graph::ResetGraph()
		{
			for (Node*& currNode : this->nodes)
			{
				currNode->visited = false;
				currNode->parent = NULL;
				currNode->gCostSoFar = FLT_MAX;
			}
		}
		void Graph::GernerateSquareMapEdges(unsigned height, unsigned width, float short_path, float long_path)
		{
			for (unsigned long y = 0; y < height - 1; y++)
			{
				unsigned long cur_row = y * width;
				unsigned long next_row = (y + 1) * width;

				//first node in row
				if (nodes[cur_row]->type != Node::wall)
				{
					if (nodes[cur_row + 1]->type != Node::wall)
					{
						AddEdge(nodes[cur_row], nodes[cur_row + 1], short_path);
					}
					if (nodes[next_row]->type != Node::wall)
					{
						AddEdge(nodes[cur_row], nodes[next_row], short_path);
					}
					if (nodes[next_row + 1]->type != Node::wall && nodes[next_row]->type != Node::wall && nodes[cur_row + 1]->type != Node::wall)
					{
						AddEdge(nodes[cur_row], nodes[next_row + 1], long_path);
					}

				}

				// across row
				for (unsigned long x = 1; x < width - 1; x++)
				{

					/*unsigned tocheck = (next_row + x) - 1;
					dynamic_cast<Component::Render*>(nodes[tocheck]->entity->GetComponent(Component::RENDER_COMPONENT))->diffuse_colour.r
						= dynamic_cast<Component::Render*>(nodes[tocheck]->entity->GetComponent(Component::RENDER_COMPONENT))->diffuse_colour.r
						== 1.f ? 0.f : 1.f;*/
					if (nodes[cur_row + x]->type != Node::wall)
					{
						if (nodes[(cur_row + x) + 1]->type != Node::wall)
						{
							AddEdge(nodes[cur_row + x], nodes[(cur_row + x) + 1], short_path);
						}
						if (nodes[next_row + x]->type != Node::wall)
						{
							AddEdge(nodes[cur_row + x], nodes[next_row + x], short_path);
						}
						if (nodes[(next_row + x) + 1]->type != Node::wall && nodes[(cur_row + x) + 1]->type != Node::wall && nodes[next_row + x]->type != Node::wall)
						{
							AddEdge(nodes[cur_row + x], nodes[(next_row + x) + 1], long_path);
						}
						if (nodes[(next_row + x) - 1]->type != Node::wall && nodes[(cur_row + x) - 1]->type != Node::wall && nodes[next_row + x]->type != Node::wall)
						{
							AddEdge(nodes[cur_row + x], nodes[(next_row + x) - 1], long_path);
						}
					}
				}

				//last node in row
				unsigned col = (width - 1);
				if (nodes[cur_row + col]->type != Node::wall)
				{
					if (nodes[next_row + col]->type != Node::wall)
					{
						AddEdge(nodes[cur_row + col], nodes[next_row + col], short_path);
					}
					if (nodes[(next_row + col) - 1]->type != Node::wall && nodes[(cur_row + col) - 1]->type != Node::wall && nodes[next_row + col]->type != Node::wall)
					{
						AddEdge(nodes[cur_row + col], nodes[(next_row + col) - 1], long_path);
					}
				}
			}

			//last row
			unsigned long cur_row = height - 1;
			for (unsigned long x = 0; x < width - 1; x++)
			{
				if (nodes[cur_row + x]->type != Node::wall && nodes[(cur_row + x) + 1]->type != Node::wall)
				{
					AddEdge(nodes[cur_row + x], nodes[(cur_row + x) + 1], short_path);
				}
			}
		}
		void Graph::AddEdge(Node* parent, Node* child, float weight, bool bUndirected)
		{
			std::pair<Node*, float> edge;
			edge.first = child;
			edge.second = weight;
			if (child->type == Node::difficult) edge.second *= 2.f;
			parent->children.push_back(edge);

			if (bUndirected)
			{
				std::pair<Node*, float> reverseEdge;
				reverseEdge.first = parent;
				reverseEdge.second = weight;
				if (parent->type == Node::difficult) reverseEdge.second *= 2.f;
				child->children.push_back(reverseEdge);
			}
		}
	}
}
