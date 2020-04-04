#include "cIntelligence.h"
#include "BMP Loader/BMPImage.h"
#include "BMP Loader/ResourceManager.h"
#include "../Entity/cEntityManager.h"
#include "../Component/Position.h"
#include "../Component/Render.h"
#include "../Component/Velocity.h"
#include "../Component/Gatherer.h"
#include "Graph.h"

ResourceManager gResourceManager;

namespace Degen
{
	namespace AI
	{
		bool IsNodeInOpenList(std::vector<Node*> openList, Node* child)
		{
			for (int i = 0; i < openList.size(); i++)
			{
				if (openList[i] == child)
					return true;

			}
			return false;
		}

		Node* Dijkstra(Node* rootNode, Graph* graph)
		{
			graph->ResetGraph();

			rootNode->visited = true;
			rootNode->gCostSoFar = 0;
			std::vector<Node*> closedList;
			std::vector<Node*> openList;
			openList.push_back(rootNode);

			while (!openList.empty())
			{
				float minCost = FLT_MAX;
				int minIndex = 0;
				Node* currNode;
				//find node with the lowest cost from root node
				for (size_t i = 0; i < openList.size(); i++)
				{
					if (openList[i]->gCostSoFar < minCost)
					{
						minCost = openList[i]->gCostSoFar;
						minIndex = i;
					}
				}

				//remove current node from open list and add to closed list
				currNode = openList[minIndex];
				for (auto iter = openList.begin(); iter != openList.end(); ++iter)
				{
					if (*iter == currNode)
					{
						openList.erase(iter);
						break;
					}
				}
				closedList.push_back(currNode);

				//cout << currNode->id << endl;
				currNode->visited = true;
				if (currNode->type == Node::resource)
				{
					return currNode;
				}

				//Go through every child node node 
				for (std::pair <Node*, float> child : currNode->children)
				{
					if (child.first->visited == false)
					{
						float weightSoFar = currNode->gCostSoFar + child.second;
						if (weightSoFar < child.first->gCostSoFar)
						{
							//update node when new better path is found
							child.first->gCostSoFar = weightSoFar;
							child.first->parent = currNode;
							if (!IsNodeInOpenList(openList, child.first))
							{
								openList.push_back(child.first); //add newly discovered node to open list
							}
						}
					}
				}
				//graph->PrintParents(true);
			}

			return NULL;
		}

		float CalculateHeuristics(Node* node, Node* goal)
		{
			float D = 1;
			float dx = abs(node->world_position.x - goal->world_position.x);
			float dy = abs(node->world_position.z - goal->world_position.z);
			return D * (dx + dy);
		}
		
		Node* AStar(Node* rootNode, Graph* graph, Node* goal)
		{
			graph->ResetGraph();

			rootNode->gCostSoFar = 0;
			rootNode->hDistance = CalculateHeuristics(rootNode, goal);

			std::vector<Node*> closedList;
			std::vector<Node*> openList;
			openList.push_back(rootNode);

			while (!openList.empty())
			{
				float minCost = FLT_MAX;
				int minIndex = 0;
				Node* currNode;
				//find node with the lowest cost from root node and heuristic distance from the goal node
				for (size_t i = 0; i < openList.size(); i++)
				{
					if (openList[i]->gCostSoFar + openList[i]->hDistance < minCost)
					{
						minCost = openList[i]->gCostSoFar + openList[i]->hDistance;
						minIndex = i;
					}
				}

				//remove current node from open list and add to closed list
				currNode = openList[minIndex];
				for (auto iter = openList.begin(); iter != openList.end(); ++iter)
				{
					if (*iter == currNode)
					{
						openList.erase(iter);
						break;
					}
				}
				closedList.push_back(currNode);

				currNode->visited = true;
				if (currNode->type == Node::home)
				{
					return currNode;
				}

				//Go through every child node node 
				for (std::pair <Node*, float> child : currNode->children)
				{
					if (child.first->visited == false)
					{
						float weightSoFar = currNode->gCostSoFar + child.second;
						if (weightSoFar < child.first->gCostSoFar)
						{
							child.first->gCostSoFar = weightSoFar;
							child.first->parent = currNode;
							if (!IsNodeInOpenList(openList, child.first))
							{
								child.first->hDistance = CalculateHeuristics(child.first, goal);
								openList.push_back(child.first);
							}
						}
					}
				}
				
			}

			return NULL;
		}

		
		cIntelligence::cIntelligence(std::string map_file) : graph(new Graph())
		{
			BMPImage* bmp = new BMPImage(map_file);

			char* data = bmp->GetData();
			map_width = bmp->GetImageWidth();
			map_height = bmp->GetImageHeight();

			int data_idx = 0;
			unsigned char r, g, b;


			for (unsigned long y = 0; y < map_height; y++)
			{
				for (unsigned long x = 0; x < map_width; x++)
				{
					b = data[data_idx];
					data_idx++;
					g = data[data_idx];
					data_idx++;
					r = data[data_idx];
					data_idx++;

					Node::node_type type = (Node::node_type) - 1;
					if (r > 125 && g > 125 && b > 125)
						type = Node::open;
					else if (r > 125 && g > 125 && b < 125)
						type = Node::difficult;
					else if (r < 125 && g < 125 && b < 125)
						type = Node::wall;
					else if (r > 125 && g < 125 && b < 125)
						type = Node::resource;
					else if (r < 125 && g < 125 && b > 125)
					{
						type = Node::home;
					}
					else if (r < 125 && g > 125 && b < 125)
					{
						type = Node::start;
					}
					else
					{
						printf("Invalid map! Colour not recognized\n");
						return;
					}

					glm::vec3 position((float)x - map_height * 0.5f, 0.f, (float)y - map_width * 0.5f);

					Node* current_node = graph->CreateNode(type, position);
					if (type == Node::home)
					{
						home_node = current_node;
					}
					if (type == Node::start)
					{
						start_node = current_node;
					}

					Entity::cEntity* ent = new Entity::cEntity();
					ent->name = "map square";
					Component::Position* pos = ent->AddComponent<Component::Position>();
					pos->position = glm::vec3((float)x - (float)map_height * 0.5f, -0.5f, (float)y - (float)map_width * 0.5f);

					if (type == Node::wall)pos->position.y = 0.5f;
					Component::Render* rend = ent->AddComponent<Component::Render>();

					rend->diffuse_colour = glm::vec4(r > 125, g > 125, b > 125, 1.f);

					rend->specular_colour = glm::vec4(1.f, 1.f, 1.f, 1.0f);
					rend->ignore_lighting = false;
					rend->mesh = "cube";
					rend->is_wireframe = false;
					rend->scale = glm::vec3(0.48f);
					rend->texture1 = "";

					Entity::cEntityManager::entities.push_back(ent);
					current_node->entity = ent;
				}
			}

			graph->GernerateSquareMapEdges(map_height, map_width, 10, 14);

		}

		void cIntelligence::Update(double dt)
		{
			for (auto* entity : entities)
			{
				Component::Gatherer* gather = dynamic_cast<Component::Gatherer*>(entity->GetComponent(Component::GATHERER_COMPONENT));
				switch (gather->current_state)
				{
					case(Component::Gatherer::search):
						SearchUpdate(dt, entity, gather);
						continue;
					case(Component::Gatherer::wait):
						WaitUpdate(dt, entity, gather);
						continue;
					case(Component::Gatherer::return_):
						ReturnUpdate(dt, entity, gather);
						continue;
					case(Component::Gatherer::idle):
						continue;
					default:
						printf("Unknown Gather State. Setting to Idle\n");
						gather->current_state = Component::Gatherer::idle;
						continue;
				}
			}
		}
		void cIntelligence::SearchUpdate(double dt, Entity::cEntity* entity, Component::Gatherer* gather)
		{
			if (gather->path.empty())
			{
				Node* target = Dijkstra(gather->node, graph);

				if(!target)
				{
					gather->current_state = Component::Gatherer::idle;
					return;
				}
				
				while(target != gather->node)
				{
					gather->path.push_back(target);
					target = target->parent;
				}
			}

			Component::Position* pos = dynamic_cast<Component::Position*>(entity->GetComponent(Component::POSITION_COMPONENT));
			Component::Velocity* vel = dynamic_cast<Component::Velocity*>(entity->GetComponent(Component::VELOCITY_COMPONENT));

			if(glm::length(vel->velocity) <= 0.1f)
			{
				vel->velocity = glm::normalize(gather->path.back()->world_position - pos->position);
			}
			
			if(glm::distance(pos->position, gather->path.back()->world_position) < 0.2)
			{
				if(gather->path.size() == 1)
				{
					gather->node = gather->path.back();
					gather->path.pop_back();
					gather->current_state = Component::Gatherer::wait;
					gather->time = 7.f;
					vel->velocity = glm::vec3(0.f);
					return;
					
				}
				gather->path.pop_back();
				
				vel->velocity = glm::normalize(gather->path.back()->world_position - pos->position);
				if(gather->path.back()->type == Node::open) vel->velocity *= 1.5;
			}

		}
		void cIntelligence::WaitUpdate(double dt, Entity::cEntity* entity, Component::Gatherer* gather)
		{
			gather->time -= dt;
			if(gather->time <= 0.f)
			{
				if (gather->node->type == Node::resource)
				{
					dynamic_cast<Component::Render*>(gather->node->entity->GetComponent(Component::RENDER_COMPONENT))
						->diffuse_colour = glm::vec4(1.f);
					gather->node->type = Node::open;

					gather->current_state = Component::Gatherer::return_;
					dynamic_cast<Component::Render*>(entity->GetComponent(Component::RENDER_COMPONENT))
						->diffuse_colour = glm::vec4(1.f, 0.f, 0.f, 1.f);
				}
				if (gather->node->type == Node::home)
				{
					gather->current_state = Component::Gatherer::search;
					dynamic_cast<Component::Render*>(entity->GetComponent(Component::RENDER_COMPONENT))
						->diffuse_colour = glm::vec4(1.f, 0.8f, 0.2f, 1.0f);
				}
			}

		}
		void cIntelligence::ReturnUpdate(double dt, Entity::cEntity* entity, Component::Gatherer* gather)
		{
			if (gather->path.empty())
			{
				Node* target = AStar(gather->node, graph, home_node);

				while (target != gather->node)
				{
					gather->path.push_back(target);
					target = target->parent;
				}
			}

			Component::Position* pos = dynamic_cast<Component::Position*>(entity->GetComponent(Component::POSITION_COMPONENT));
			Component::Velocity* vel = dynamic_cast<Component::Velocity*>(entity->GetComponent(Component::VELOCITY_COMPONENT));

			if (glm::length(vel->velocity) <= 0.1f)
			{
				vel->velocity = glm::normalize(gather->path.back()->world_position - pos->position);
			}

			if (glm::distance(pos->position, gather->path.back()->world_position) < 0.2)
			{
				if (gather->path.size() == 1)
				{
					gather->node = gather->path.back();
					gather->path.pop_back();
					gather->current_state = Component::Gatherer::wait;
					gather->time = 2.f;
					vel->velocity = glm::vec3(0.f);
					return;

				}
				gather->path.pop_back();

				vel->velocity = glm::normalize(gather->path.back()->world_position - pos->position);
			}
		}
		void cIntelligence::AddEntity(Entity::cEntity* entity)
		{
			if (entity->HasComponent(Component::GATHERER_COMPONENT) &&
				entity->HasComponent(Component::VELOCITY_COMPONENT) &&
				entity->HasComponent(Component::POSITION_COMPONENT))
			{
				if (std::find(entities.begin(), entities.end(), entity) == entities.end())
				{
					Component::Gatherer* g = dynamic_cast<Component::Gatherer*>(entity->GetComponent(Component::GATHERER_COMPONENT));
					g->node = start_node;
					g->current_state = Component::Gatherer::search;
					g->time = 0.f;
					g->path.clear();
					Component::Position* p = dynamic_cast<Component::Position*>(entity->GetComponent(Component::POSITION_COMPONENT));
					p->position = start_node->world_position;
					entities.push_back(entity);
				}
			}
		}

	}
}

