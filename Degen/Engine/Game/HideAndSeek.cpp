#include "HideAndSeek.h"
#include <vector>
#include <glm.hpp>
#include "../Globals.h"
#include "../Component/Render.h"
#include "../Component/MultiTransform.h"
#include "../Component/Radius2D.h"
#include "../Component/Position.h"
#include "../Component/Animation_New.h"
#include "../Component/Firefly.h"
#include "../Component/Light.h"
#include "../Component/Motion.h"

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

			Component::Radius2D* collide = dynamic_cast<Component::Radius2D*>(trees_collide->AddComponent<Component::Radius2D>());
			collide->radius = 2.5f;
			tree_collisions = trees_collide;


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

			for (int i = 0; i < 20; i++)
			{
				Entity::cEntity* firefly = EntityManager->CreateEntity();
				firefly->user_id = 10;
				Component::Position* firefly_pos_comp = dynamic_cast<Component::Position*>(firefly->AddComponent<Component::Position>());
				firefly_pos_comp->position = glm::vec3((rand() % 200) - 100, (rand() % 5) + 3, (rand() % 200) - 100);

				Component::Motion* firefly_motion_comp = dynamic_cast<Component::Motion*>(firefly->AddComponent<Component::Motion>());
				firefly_motion_comp->max_velocity = 2.f;


				Component::Radius2D* firefly_radius_comp = dynamic_cast<Component::Radius2D*>(firefly->AddComponent<Component::Radius2D>());
				firefly_radius_comp->radius = 0.5f;

				Component::Render* firefly_rend_comp = dynamic_cast<Component::Render*>(firefly->AddComponent<Component::Render>());
				{
					firefly_rend_comp->mesh = "sphere low";
					firefly_rend_comp->texture1 = "Wall Dark";
					firefly_rend_comp->texture1_amount = 0.f;
					firefly_rend_comp->diffuse_colour = glm::vec4(1.f, 1.f, 0.1, 1.f);
					firefly_rend_comp->diffuse_amount = 1.2f;
					firefly_rend_comp->specular_colour = glm::vec4(1.f, 1.f, 1.f, 100.f);
					firefly_rend_comp->scale = glm::vec3(0.1, 0.1, 0.1);
					firefly_rend_comp->ignore_lighting = false;
					firefly_rend_comp->is_wireframe = false;
					firefly_rend_comp->tesselate = false;
				}
				Component::Firefly* firefly_firefly_comp = dynamic_cast<Component::Firefly*>(firefly->AddComponent<Component::Firefly>());
				Component::Light* firefly_light_comp = dynamic_cast<Component::Light*>(firefly->AddComponent<Component::Light>());
				{
					firefly_light_comp->diffuse = glm::vec4(1.0, 1.0, 0.5, 1.0);
					firefly_light_comp->specular = glm::vec4(1.0, 1.0, 1.0, 1.0);
					firefly_light_comp->const_atten = 0.00001;
					firefly_light_comp->linear_atten = 0.3;
					firefly_light_comp->quad_atten = 0.004;
					firefly_light_comp->cutoff = 10;
					firefly_light_comp->type = Component::Light::point_light;
					firefly_light_comp->on = true;
				}

				fireflies.push_back(firefly);
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

			for (auto firefly : fireflies)
			{
				Component::Motion* firefly_motion_comp = dynamic_cast<Component::Motion*>(firefly->GetComponent(Component::MOTION_COMPONENT));
				firefly_motion_comp->velocity += glm::vec3(((float)(rand() % 100) - 50) / 50.f, ((float)(rand() % 100) - 50) / 50.f, ((float)(rand() % 100) - 50) / 50.f);
			}


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
				Component::Position* statue_pos = dynamic_cast<Component::Position*>(statue->GetComponent(Component::POSITION_COMPONENT));
				if (glm::distance(statue_pos->position, player_pos->position) < 15.f)
				{
					glm::vec3 look = View->target - View->position;
					look.y = 0.f;
					look = glm::normalize(look);

					glm::vec3 to_statue = statue_pos->position - View->position;
					to_statue.y = 0.f;
					to_statue = glm::normalize(to_statue);

					if (glm::dot(look, to_statue) > 0.3) TextRenderer->AddText("[E] -> Interact", 25, 75, 1, glm::vec3(1), 0.f);

				}
			}

		}
		void HideAndSeek::InteractFriend()
		{
			if (seeking)
			{
				Component::Animation_New* other_ani = dynamic_cast<Component::Animation_New*>(goblin->GetComponent(Component::ANIMATION_NEW_COMPONENT));

				TextRenderer->AddText("YAY! You found me! Go back to the statue to go again.", WINDOW_WIDTH * 0.35, 250, 1, glm::vec3(1), 4);

				Component::animation_info ai;
				ai.play_till_end = true;
				ai.blend_for = 0.2f;
				ai.animation = "forest_guard@standup";
				other_ani->animation_queue.push_back(ai);
				ai.animation = "forest_guard@clapping";
				other_ani->animation_queue.push_back(ai);

				ai.play_till_end = false;
				ai.animation = "forest_guard@idle_still";
				other_ani->animation_queue.push_back(ai);

				seeking = false;
			}
			else
			{
				TextRenderer->AddText("Go to the transparent statue and count and I will hide!", WINDOW_WIDTH * 0.35, 250, 1, glm::vec3(1), 4);

			}

		}
		void HideAndSeek::InteractStatue()
		{
			TextRenderer->AddText("3...2...1... Ready or not here I come", WINDOW_WIDTH * 0.4, 250, 1, glm::vec3(1), 4);

			Component::Animation_New* other_ani = dynamic_cast<Component::Animation_New*>(goblin->GetComponent(Component::ANIMATION_NEW_COMPONENT));

			Component::animation_info ai;
			ai.play_till_end = false;
			ai.blend_for = 0.2f;
			ai.animation = "forest_guard@idle_crouch";
			other_ani->animation_queue.push_back(ai);

			Component::Position* goblin_pos = dynamic_cast<Component::Position*>(goblin->GetComponent(Component::POSITION_COMPONENT));

			bool spot_found = false;
			do
			{
				goblin_pos->position.x = (rand() % 380) - 190;
				goblin_pos->position.z = (rand() % 380) - 190;
				if (glm::length(goblin_pos->position) <= 60)
				{
					continue;
				}

				spot_found = true;
				Component::MultiTransform* transform = dynamic_cast<Component::MultiTransform*>(tree_collisions->GetComponent(Component::MULTI_TRANSFORM_COMPONENT));
				for (auto trans : transform->transforms)
				{
					if(glm::distance(glm::vec3(trans[3].x,0,trans[3].z), goblin_pos->position) < 4.f)
					{
						spot_found = false;
						break;
					}
				}

			} while (!spot_found);


			seeking = true;
		}
	}
}
