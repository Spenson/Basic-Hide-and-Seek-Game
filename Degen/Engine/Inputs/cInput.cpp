#include "cInput.h"
#include "../Globals.h"
#include "../Component/Position.h"
#include "../Component/Camera.h"
#include "../Component/Animation_New.h"
#include "../Component/Motion.h"
#include "../Component/Rotation.h"
#include "../Game/HideAndSeek.h"

namespace Degen
{
	namespace Input
	{
		bool cInput::mouse_on_window = false;
		double cInput::mouse_scroll_x_offset = 0;
		double cInput::mouse_scroll_y_offset = 0;

		float wait = 0;

		void cursor_enter_callback(GLFWwindow* window, int entered)
		{
			if (entered)
			{
				cInput::mouse_on_window = true;
			}
			else
			{
				cInput::mouse_on_window = false;
			}
			return;
		}

		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
		{
			cInput::mouse_scroll_x_offset += xoffset;
			cInput::mouse_scroll_y_offset += yoffset;
		}

		cInput::cInput(GLFWwindow* window) : window(window)
		{
			glfwSetCursorEnterCallback(window, cursor_enter_callback);
			glfwSetScrollCallback(window, scroll_callback);
		}


		void cInput::Update(double dt)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			/*if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) &&mouse_on_window)*/
			{
				float diffX = x - mouse_position_x;
				float diffY = y - mouse_position_y;
				Component::Camera* camera = dynamic_cast<Component::Camera*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::CAMERA_COMPONENT));
				camera->pitch -= (diffY * 0.5f);
				camera->yaw -= (diffX * 0.5f);

				camera->distance -= mouse_scroll_y_offset;

			}
			mouse_scroll_x_offset = 0;
			mouse_scroll_y_offset = 0;
			mouse_position_x = x;
			mouse_position_y = y;

			//if (glfwGetKey(window, GLFW_KEY_W))
			//{
			//	glm::vec3 movement = View->target - View->position;
			//	movement.y = 0.f;
			//	movement = glm::normalize(movement);
			//	movement *= 20.f * dt;
			//	dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position += movement;
			//}
			//else if (glfwGetKey(window, GLFW_KEY_S))
			//{
			//	glm::vec3 movement = View->target - View->position;
			//	movement.y = 0.f;
			//	movement = glm::normalize(movement);
			//	movement *= 20.f * dt;
			//	dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position -= movement;
			//}
			//if (glfwGetKey(window, GLFW_KEY_A))
			//{
			//	glm::vec3 movement = glm::cross(View->target - View->position, View->up);
			//	movement.y = 0.f;
			//	movement = glm::normalize(movement);
			//	movement *= 20.f * dt;
			//	dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position -= movement;
			//}
			//else if (glfwGetKey(window, GLFW_KEY_D))
			//{
			//	glm::vec3 movement = glm::cross(View->target - View->position, View->up);
			//	movement.y = 0.f;
			//	movement = glm::normalize(movement);
			//	movement *= 20.f * dt;
			//	dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position += movement;
			//}
			//if (glfwGetKey(window, GLFW_KEY_Q))
			//{
			//	glm::vec3 movement(0.f);
			//	movement.y = 1.f;
			//	movement = glm::normalize(movement);
			//	movement *= 20.f * dt;
			//	dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position -= movement;
			//}
			//else if (glfwGetKey(window, GLFW_KEY_E))
			//{
			//	glm::vec3 movement(0.f);
			//	movement.y = 1.f;
			//	movement = glm::normalize(movement);
			//	movement *= 20.f * dt;
			//	dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position += movement;
			//}


			Entity::cEntity* player = EntityManager->GetEntity("Elf");

			Component::Animation_New* animation_comp = dynamic_cast<Component::Animation_New*>(player->GetComponent(Component::ANIMATION_NEW_COMPONENT));
			Component::Motion* motion_comp = dynamic_cast<Component::Motion*>(player->GetComponent(Component::MOTION_COMPONENT));
			Component::Rotation* rotation_comp = dynamic_cast<Component::Rotation*>(player->GetComponent(Component::ROTATION_COMPONENT));

			float speed = glm::length(motion_comp->velocity);
			float adjust = motion_comp->max_velocity * 0.1f;
			if (glfwGetKey(window, GLFW_KEY_W))
			{

				glm::vec3 dir = View->target - View->position;
				dir.y = 0.f;
				dir = glm::normalize(dir);

				rotation_comp->rotation = glm::quatLookAt(-dir, glm::vec3(0, 1, 0));

				speed += adjust;
				motion_comp->velocity = dir * speed;


				if (animation_comp->animation_queue.empty())
				{
					Component::animation_info info;
					info.animation = "elf@walk";
					info.play_till_end = true;
					info.blend_for = 0.5f;
					animation_comp->animation_queue.push_back(info);
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_D))
			{
				glm::vec3 dir = glm::cross(View->target - View->position, View->up);
				dir.y = 0.f;
				dir = glm::normalize(dir);

				glm::vec3 look = View->target - View->position;
				look.y = 0.f;
				look = glm::normalize(look);
				rotation_comp->rotation = glm::quatLookAt(-look, glm::vec3(0, 1, 0));

				speed += adjust;
				motion_comp->velocity = dir * speed;


				if (animation_comp->animation_queue.empty())
				{
					if (animation_comp->animation == "elf@strafe_left" || animation_comp->animation == "elf@idle_left")
					{
						Component::animation_info idleinfo;
						idleinfo.animation = "elf@idle";
						idleinfo.play_till_end = false;
						idleinfo.blend_for = 0.5f;
						animation_comp->animation_queue.push_back(idleinfo);

					}

					Component::animation_info info;
					info.animation = "elf@strafe_right";
					info.play_till_end = false;
					info.blend_for = 0.5f;
					animation_comp->animation_queue.push_back(info);
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_A))
			{
				glm::vec3 dir = glm::cross(View->target - View->position, View->up);
				dir.y = 0.f;
				dir = glm::normalize(dir);

				glm::vec3 look = View->target - View->position;
				look.y = 0.f;
				look = glm::normalize(look);
				rotation_comp->rotation = glm::quatLookAt(-look, glm::vec3(0, 1, 0));

				speed += adjust;
				motion_comp->velocity = -dir * speed;

				if (animation_comp->animation_queue.empty())
				{
					if (animation_comp->animation == "elf@strafe_right")
					{
						Component::animation_info idleinfo;
						idleinfo.animation = "elf@idle";
						idleinfo.play_till_end = false;
						idleinfo.blend_for = 0.25f;
						animation_comp->animation_queue.push_back(idleinfo);
						idleinfo.animation = "elf@idle_left";
						idleinfo.play_till_end = false;
						idleinfo.blend_for = 0.25f;
						animation_comp->animation_queue.push_back(idleinfo);

					}
					else if (animation_comp->animation == "elf@idle")
					{
						Component::animation_info idleinfo;
						idleinfo.animation = "elf@idle_left";
						idleinfo.play_till_end = false;
						idleinfo.blend_for = 0.5f;
						animation_comp->animation_queue.push_back(idleinfo);

					}
					Component::animation_info info;
					info.animation = "elf@strafe_left";
					info.play_till_end = false;
					info.blend_for = 0.5f;
					animation_comp->animation_queue.push_back(info);
				}
			}
			else
			{
				if (speed > 0)
				{
					glm::vec3 dir = glm::normalize(motion_comp->velocity);

					speed -= adjust;

					if (speed < 0) speed = 0;

					motion_comp->velocity = dir * speed;

				}
				if (animation_comp->animation_queue.empty())
				{
					if (animation_comp->animation == "elf@strafe_left")
					{
						Component::animation_info idleinfo;
						idleinfo.animation = "elf@idle_left";
						idleinfo.play_till_end = false;
						idleinfo.blend_for = 0.5f;
						animation_comp->animation_queue.push_back(idleinfo);
					}

					Component::animation_info idleinfo;
					idleinfo.animation = "elf@idle";
					idleinfo.play_till_end = false;
					idleinfo.blend_for = 0.5f;
					animation_comp->animation_queue.push_back(idleinfo);

				}
			}

			if (glfwGetKey(window, GLFW_KEY_E) && wait <= 0.f)
			{
				wait = 1.f;
				
				Component::Position* player_pos = dynamic_cast<Component::Position*>(player->GetComponent(Component::POSITION_COMPONENT));
				Entity::cEntity* goblin = EntityManager->GetEntity("forest guard");

				
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
					{
						Engine->mHideAndSeek->InteractFriend();
						return;
					}
				}
				
				if (!seeking)
				{
					Entity::cEntity* statue = EntityManager->GetEntity("Statue");

					Component::Position* statue_pos = dynamic_cast<Component::Position*>(statue->GetComponent(Component::POSITION_COMPONENT));
					if (glm::distance(statue_pos->position, player_pos->position) < 15.f)
					{
						glm::vec3 look = View->target - View->position;
						look.y = 0.f;
						look = glm::normalize(look);

						glm::vec3 to_statue = statue_pos->position - View->position;
						to_statue.y = 0.f;
						to_statue = glm::normalize(to_statue);

						if (glm::dot(look, to_statue) > 0.3) 
						{
							Engine->mHideAndSeek->InteractStatue();
							return;
						}
					}
				}


			}
			wait -= dt;
		}
	}
}
