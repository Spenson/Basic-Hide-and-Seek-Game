#include "cIntelligence.h"
#include "BMP Loader/BMPImage.h"
#include "BMP Loader/ResourceManager.h"
#include "../Entity/cEntityManager.h"
#include "../Component/Position.h"
#include "../Component/Render.h"

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
					r = data[data_idx++];
					g = data[data_idx++];
					b = data[data_idx++];

					map[map_idx] = new node();

					map[map_idx]->world_position = glm::vec3((float)x - map_height * 0.5f, 0.f, (float)y - map_width * 0.5f);
					map[map_idx]->x = x;
					map[map_idx]->y = y;

					if (r == 255 && g == 255 && b == 255)
						map[map_idx]->type = open;
					else if (r == 255 && g == 255 && b == 0)
						map[map_idx]->type = difficult;
					else if (r == 0 && g == 0 && b == 0)
						map[map_idx]->type = wall;
					else if (r == 0 && g == 255 && b == 0)
						map[map_idx]->type = start;
					else if (r == 255 && g == 0 && b == 0)
						map[map_idx]->type = resource;
					else if (r == 0 && g == 0 && b == 255)
						map[map_idx]->type = home;

					Entity::cEntity* ent = new Entity::cEntity();
					ent->name = "map square";
					Component::Position* pos = ent->AddComponent<Component::Position>();
					pos->position = glm::vec3((float)x - (float)map_height * 0.5f, -0.5f, (float)y - (float)map_width * 0.5f);
					if (map[map_idx]->type == wall)pos->position.y = 0.5f;
					Component::Render* rend = ent->AddComponent<Component::Render>();

					rend->diffuse_colour = glm::vec4(!!r, !!g, !!b, 1.f);


					rend->specular_colour = glm::vec4(1.f, 1.f, 1.f, .5f);
					rend->ignore_lighting = false;
					rend->mesh = "cube";
					rend->is_wireframe = false;
					rend->scale = glm::vec3(0.5f);
					rend->texture = "";

					Entity::cEntityManager::entities.push_back(ent);

				}

			}

		}

		void cIntelligence::Update(double dt)
		{
		}

		void cIntelligence::AddEntity(Entity::cEntity* entity)
		{
		}

	}
}

