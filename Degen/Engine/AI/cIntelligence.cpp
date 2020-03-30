#include "cIntelligence.h"
#include "BMP Loader/BMPImage.h"
#include "BMP Loader/ResourceManager.h"
#include "../Entity/cEntityManager.h"
#include "../Component/Position.h"
#include "../Component/Render.h"
#include "../Component/Velocity.h"
#include "../Component/Gatherer.h"

ResourceManager gResourceManager;

namespace Degen
{
	namespace AI
	{
		cIntelligence::cIntelligence(std::string map_file)
		{
			BMPImage* bmp = new BMPImage(map_file);

			char* data = bmp->GetData();
			map_width = bmp->GetImageWidth();
			map_height = bmp->GetImageHeight();
			unsigned long isize = bmp->GetImageSize();

			map.resize(isize / 3);



			int data_idx = 0;
			int map_idx = 0;
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

					map[map_idx] = new node();

					map[map_idx]->world_position = glm::vec3((float)x - map_height * 0.5f, 0.f, (float)y - map_width * 0.5f);
					map[map_idx]->x = x;
					map[map_idx]->y = y;

					if (r > 125 && g > 125 && b > 125)
						map[map_idx]->type = open;
					else if (r > 125 && g > 125 && b < 125)
						map[map_idx]->type = difficult;
					else if (r < 125 && g < 125 && b < 125)
						map[map_idx]->type = wall;
					else if (r > 125 && g < 125 && b < 125)
						map[map_idx]->type = resource;
					else if (r < 125 && g < 125 && b > 125)
					{
						map[map_idx]->type = home;
						home_node = map_idx;
					}
					else if (r < 125 && g > 125 && b < 125)
						map[map_idx]->type = start;
					else
					{
						printf("Invalid map! Colour not recognized\n");
						return;
					}


					Entity::cEntity* ent = new Entity::cEntity();
					ent->name = "map square";
					Component::Position* pos = ent->AddComponent<Component::Position>();
					pos->position = glm::vec3((float)x - (float)map_height * 0.5f, -0.5f, (float)y - (float)map_width * 0.5f);
					if (map[map_idx]->type == wall)pos->position.y = 0.5f;
					Component::Render* rend = ent->AddComponent<Component::Render>();

					rend->diffuse_colour = glm::vec4(r > 125, g > 125, b > 125, 1.f);

					rend->specular_colour = glm::vec4(1.f, 1.f, 1.f, 1.0f);
					rend->ignore_lighting = false;
					rend->mesh = "cube";
					rend->is_wireframe = false;
					rend->scale = glm::vec3(0.45f);
					rend->texture = "";

					Entity::cEntityManager::entities.push_back(ent);
					map_idx++;
				}
			}



			edge* e;
			for (unsigned long y = 0; y < map_height - 1; y++)
			{
				unsigned long cur_row = y * map_width;
				unsigned long next_row = (y + 1) * map_width;

				if (map[cur_row]->type != wall)
				{//first node in row
					if (map[cur_row + 1]->type != wall)
					{
						e = new edge(map[cur_row], map[cur_row + 1], 10);
						map[cur_row]->edges.push_back(e);
						map[cur_row + 1]->edges.push_back(e);
					}
					if (map[next_row]->type != wall)
					{
						e = new edge(map[cur_row], map[next_row], 10);
						map[cur_row]->edges.push_back(e);
						map[next_row]->edges.push_back(e);
					}
					if (map[next_row + 1]->type != wall && map[next_row]->type != wall && map[cur_row + 1]->type != wall)
					{
						e = new edge(map[cur_row], map[next_row], 14);
						map[cur_row]->edges.push_back(e);
						map[next_row + 1]->edges.push_back(e);
					}
					/*
					printf("(%f,%f,%f)\n\t(%f,%f,%f)\n\t(%f,%f,%f)\n\t(%f,%f,%f)\n",
						   map[cur_row]->world_position.x, map[cur_row]->world_position.y, map[cur_row]->world_position.z,
						   map[cur_row + 1]->world_position.x, map[cur_row + 1]->world_position.y, map[cur_row + 1]->world_position.z,
						   map[next_row]->world_position.x, map[next_row]->world_position.y, map[next_row]->world_position.z,
						   map[next_row + 1]->world_position.x, map[next_row + 1]->world_position.y, map[next_row + 1]->world_position.z
					);
					//*/
				}

				// across row
				for (unsigned long x = 1; x < map_width - 1; x++)
				{
					if (map[cur_row + x]->type != wall)
					{
						if (map[(cur_row + x) + 1]->type != wall)
						{
							e = new edge(map[cur_row + x], map[(cur_row + x) + 1], 10);
							map[cur_row + x]->edges.push_back(e);
							map[(cur_row + x) + 1]->edges.push_back(e);
						}
						if (map[next_row + x]->type != wall)
						{
							e = new edge(map[cur_row + x], map[next_row + x], 10);
							map[cur_row + x]->edges.push_back(e);
							map[next_row + x]->edges.push_back(e);
						}
						if (map[(next_row + x) + 1]->type != wall && map[(cur_row + x) + 1]->type != wall && map[next_row + x]->type != wall)
						{
							e = new edge(map[cur_row + x], map[(next_row + x) + 1], 14);
							map[cur_row + x]->edges.push_back(e);
							map[(next_row + x) + 1]->edges.push_back(e);
						}


						if (map[(next_row + x) - 1]->type != wall && map[(cur_row + x) - 1]->type != wall && map[next_row + x]->type != wall)
						{
							e = new edge(map[cur_row + x], map[(next_row + x) - 1], 14);
							map[cur_row + x]->edges.push_back(e);
							map[(next_row + x) - 1]->edges.push_back(e);
						}
						/*
						printf("(%f,%f,%f)\n\t(%f,%f,%f)\n\t(%f,%f,%f)\n\t(%f,%f,%f)\n\t(%f,%f,%f)\n",
							   map[cur_row + x]->world_position.x, map[cur_row + x]->world_position.y, map[cur_row + x]->world_position.z,
							   map[(cur_row + x) + 1]->world_position.x, map[(cur_row + x) + 1]->world_position.y, map[(cur_row + x) + 1]->world_position.z,
							   map[next_row + x]->world_position.x, map[next_row + x]->world_position.y, map[next_row + x]->world_position.z,
							   map[(next_row + x) + 1]->world_position.x, map[(next_row + x) + 1]->world_position.y, map[(next_row + x) + 1]->world_position.z,
							   map[(next_row + x) - 1]->world_position.x, map[(next_row + x) - 1]->world_position.y, map[(next_row + x) - 1]->world_position.z
						);
						//*/
					}
				}

				if (map[cur_row + (map_width - 1)]->type != wall)
				{//last node in row
					if (map[next_row + (map_width - 1)]->type != wall)
					{
						e = new edge(map[cur_row + (map_width - 1)], map[next_row + (map_width - 1)], 10);
						map[cur_row + (map_width - 1)]->edges.push_back(e);
						map[next_row + (map_width - 1)]->edges.push_back(e);
					}
					if (map[(next_row + (map_width - 1)) - 1]->type != wall && map[(cur_row + (map_width - 1)) - 1]->type != wall && map[next_row + (map_width - 1)]->type != wall)
					{
						e = new edge(map[cur_row + (map_width - 1)], map[(next_row + (map_width - 1)) - 1], 14);
						map[cur_row + (map_width - 1)]->edges.push_back(e);
						map[(next_row + (map_width - 1)) - 1]->edges.push_back(e);
					}
				}
			}

			//last row
			unsigned long cur_row = map_width - 1;
			for (unsigned long x = 0; x < map_width - 1; x++)
			{
				if (map[cur_row + x]->type != wall && map[(cur_row + x) + 1]->type != wall)
				{
					e = new edge(map[cur_row + x], map[(cur_row + x) + 1], 10);
					map[cur_row + x]->edges.push_back(e);
					map[(cur_row + x) + 1]->edges.push_back(e);
				}
			}


		}

		void cIntelligence::Update(double dt)
		{
		}

		void cIntelligence::AddEntity(Entity::cEntity* entity)
		{
			if (entity->HasComponent(Component::GATHERER_COMPONENT) &&
				entity->HasComponent(Component::VELOCITY_COMPONENT) &&
				entity->HasComponent(Component::POSITION_COMPONENT))
			{
				if (std::find(entities.begin(), entities.end(), entity) == entities.end())
				{
					entities.push_back(entity);
				}
			}
		}

	}
}

