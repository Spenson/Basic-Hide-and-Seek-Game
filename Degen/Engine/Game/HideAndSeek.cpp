#include "HideAndSeek.h"
#include <vector>
#include <glm.hpp>
#include "../Globals.h"
#include "../Component/Render.h"
#include "../Component/MultiTransform.h"
#include "../Component/Radius2D.h"
#include "../Component/Position.h"
#include "../Component/Animation_New.h"

namespace Degen
{

	bool start = true;
	bool seeking = false;
	namespace Game
	{
		HideAndSeek::HideAndSeek()
		{
			std::vector<glm::vec3> positions;

			for (size_t c = 0; c < 100; c++)
			{
				for (;;)
				{
					glm::vec3 pos(((rand() % 4000) - 2000) / 10.f, 24.5f, ((rand() % 4000) - 2000) / 10.f);
					while (glm::length(pos) <= 45)
					{
						pos = glm::vec3(((float)(rand() % 4000) - 2000) / 10.f, 24.5f, ((float)(rand() % 4000) - 2000) / 10.f);
					}

					bool overlap = false;
					for (auto other : positions)
					{
						if (glm::distance(pos, other) < 15.f)
						{
							overlap = true;
							break;
						}
					}
					if (overlap) continue;

					positions.push_back(pos);
					break;
				}
			}

			Entity::cEntity* trees_collide = EntityManager->CreateEntity();
			Component::MultiTransform* collide_transforms = dynamic_cast<Component::MultiTransform*>(trees_collide->AddComponent<Component::MultiTransform>());

			Component::Render* collide_rend_comp = dynamic_cast<Component::Render*>(trees_collide->AddComponent<Component::Render>());
			{
				collide_rend_comp->mesh = "sphere low";
				collide_rend_comp->texture1 = "Bark colour";
				collide_rend_comp->texture1_amount = 0.f;
				collide_rend_comp->diffuse_colour = glm::vec4(1.f);
				collide_rend_comp->diffuse_amount = 1.f;
				collide_rend_comp->specular_colour = glm::vec4(1.f, 1.f, 1.f, 100.f);
				collide_rend_comp->scale = glm::vec3(3, 1, 3);
				collide_rend_comp->ignore_lighting = true;
				collide_rend_comp->is_wireframe = false;
				collide_rend_comp->tesselate = false;
			}
			Component::Radius2D* collide = dynamic_cast<Component::Radius2D*>(trees_collide->AddComponent<Component::Radius2D>());
			collide->radius = 2.5f;



			Entity::cEntity* trees = EntityManager->CreateEntity();
			Component::Render* rend_comp = dynamic_cast<Component::Render*>(trees->AddComponent<Component::Render>());
			{
				rend_comp->mesh = "Tree leaf";
				rend_comp->texture1 = "Leaf colour";
				rend_comp->texture1_amount = 1.f;
				rend_comp->texture2 = "Leaf alpha";
				rend_comp->diffuse_amount = 0.f;
				rend_comp->specular_colour = glm::vec4(1.f, 1.f, 1.f, 100.f);
				rend_comp->scale = glm::vec3(0.05, 0.06, 0.05);
				rend_comp->ignore_lighting = false;
				rend_comp->is_wireframe = false;
				rend_comp->cull_face_back = false;
				rend_comp->alpha_mode = 2;
				rend_comp->use_bump_map = true;
				rend_comp->bump_map = "Leaf normal";
				rend_comp->tesselate = false;
			}
			//rend_comp = dynamic_cast<Component::Render*>(trees->AddComponent<Component::Render>());
			//{
			//	rend_comp->mesh = "Tree leaf";
			//	rend_comp->texture1 = "Leaf colour";
			//	rend_comp->texture1_amount = 1.f;
			//	rend_comp->texture2 = "Leaf alpha";
			//	rend_comp->diffuse_amount = 0.f;
			//	rend_comp->specular_colour = glm::vec4(1.f, 1.f, 1.f, 100.f);
			//	rend_comp->scale = glm::vec3(0.06, 0.07, 0.07);
			//	rend_comp->ignore_lighting = false;
			//	rend_comp->is_wireframe = false;
			//	rend_comp->cull_face_back = false;
			//	rend_comp->alpha_mode = 2;
			//	rend_comp->use_bump_map = true;
			//	rend_comp->bump_map = "Leaf normal";
			//	rend_comp->tesselate = false;
			//}
			//rend_comp = dynamic_cast<Component::Render*>(tress->AddComponent<Component::Render>());
			//{
			//	rend_comp->mesh = "Tree leaf";
			//	rend_comp->texture1 = "Leaf colour";
			//	rend_comp->texture1_amount = 1.f;
			//	rend_comp->texture2 = "Leaf alpha";
			//	rend_comp->diffuse_amount = 0.f;
			//	rend_comp->specular_colour = glm::vec4(1.f, 1.f, 1.f, 100.f);
			//	rend_comp->scale = glm::vec3(0.07, 0.08, 0.08);
			//	rend_comp->ignore_lighting = false;
			//	rend_comp->is_wireframe = false;
			//	rend_comp->cull_face_back = false;
			//	rend_comp->alpha_mode = 2;
			//	rend_comp->use_bump_map = true;
			//	rend_comp->bump_map = "Leaf normal";
			//rend_comp->tesselate = false;
			//}

			rend_comp = dynamic_cast<Component::Render*>(trees->AddComponent<Component::Render>());
			{
				rend_comp->mesh = "Tree bark";
				rend_comp->texture1 = "Bark colour";
				rend_comp->texture1_amount = 1.f;
				rend_comp->diffuse_colour = glm::vec4(1.f);
				rend_comp->diffuse_amount = 0.f;
				rend_comp->specular_colour = glm::vec4(1.f, 1.f, 1.f, 100.f);
				rend_comp->scale = glm::vec3(0.04, 0.05, 0.04);
				rend_comp->ignore_lighting = false;
				rend_comp->is_wireframe = false;
				rend_comp->use_bump_map = true;
				rend_comp->bump_map = "Bark normal";
				rend_comp->tesselate = false;
			}

			Component::MultiTransform* tree_transforms = dynamic_cast<Component::MultiTransform*>(trees->AddComponent<Component::MultiTransform>());


			glm::mat4 trans_tree = glm::mat4(1.f);
			glm::mat4 trans_collide = glm::mat4(1.f);
			glm::quat rot;
			for (auto p : positions)
			{
				rot = glm::quat(glm::radians(glm::vec3(0, (float)(rand() % 3600) / 10.f, 0)));

				trans_tree = glm::mat4(1.f);
				trans_tree = glm::translate(trans_tree, p);
				trans_tree *= glm::mat4(rot);

				trans_collide = glm::mat4(1.f);
				glm::vec3 offset = glm::mat4(rot) * glm::vec4(-6.5f, -24.5, 0.f, 0.f);
				trans_collide = glm::translate(trans_collide, p + offset);
				trans_collide *= glm::mat4(rot);

				tree_transforms->transforms.push_back(trans_tree);
				collide_transforms->transforms.push_back(trans_collide);
			}


			TextRenderer->AddText("[Esc] -> Exit", 25, 175, 1, glm::vec3(1), -1);
			TextRenderer->AddText("[W][A][D] -> Move", 25, 125, 1, glm::vec3(1), -1);
			//TextRenderer->AddText("[E] -> Interact", 25, 75, 1, glm::vec3(1), -1);


			//TextRenderer->AddText("3...2...1... Ready or not here I come", WINDOW_WIDTH*0.4, 250, 1, glm::vec3(1), -1);
			//TextRenderer->AddText("Go to the transparent statue and count and I will hide!", WINDOW_WIDTH*0.35, 250, 1, glm::vec3(1), -1);
			//TextRenderer->AddText("YAY! You found me! Go back to the statue to go again.", WINDOW_WIDTH * 0.35, 250, 1, glm::vec3(1), -1);

		}
		void HideAndSeek::AddEntity(Entity::cEntity* entity)
		{
			if (entity->user_id == 3)
			{
				player = entity;
			}
			else if (entity->user_id == 1)
			{
				statue = entity;
			}
			else if (entity->user_id == 2)
			{
				goblin = entity;
			}
			else if (entity->user_id == 1000)
			{
				camera = entity;
			}
		}
		void HideAndSeek::Update(double dt)
		{
			Component::Position* cam_pos = dynamic_cast<Component::Position*>(camera->GetComponent(Component::POSITION_COMPONENT));
			Component::Position* player_pos = dynamic_cast<Component::Position*>(player->GetComponent(Component::POSITION_COMPONENT));
			cam_pos->position = player_pos->position;
			cam_pos->position.y = 5.5f;


			Component::Position* goblin_pos = dynamic_cast<Component::Position*>(goblin->GetComponent(Component::POSITION_COMPONENT));
			if (glm::distance(goblin_pos->position, player_pos->position) < 10.f)
			{
				glm::vec3 look = View->target - View->position;
				look.y = 0.f;
				look = glm::normalize(look);

				glm::vec3 to_goblin = goblin_pos->position - View->position;
				to_goblin.y = 0.f;
				to_goblin = glm::normalize(to_goblin);

				if (glm::dot(look, to_goblin) > 0.3)
					TextRenderer->AddText("[E] -> Interact", 25, 75, 1, glm::vec3(1), 0.f);

			}
			if (!seeking)
			{
				Component::Position* goblin_pos = dynamic_cast<Component::Position*>(goblin->GetComponent(Component::POSITION_COMPONENT));
				if (glm::distance(goblin_pos->position, player_pos->position) < 10.f)
				{
					glm::vec3 look = View->target - View->position;
					look.y = 0.f;
					look = glm::normalize(look);

					glm::vec3 to_goblin = goblin_pos->position - View->position;
					to_goblin.y = 0.f;
					to_goblin = glm::normalize(to_goblin);

					if (glm::dot(look, to_goblin) > 0.3)
						TextRenderer->AddText("[E] -> Interact", 25, 75, 1, glm::vec3(1), 0.f);

				}
			}
		}
	}
}
