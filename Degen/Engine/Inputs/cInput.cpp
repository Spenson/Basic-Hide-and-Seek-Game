#include "cInput.h"
#include "../Globals.h"
#include "../Component/Position.h"
#include "../Component/Camera.h"
#include "../Component/Animation.h"
#include "../Component/Physics.h"
#include <physics/interfaces/iLauncherComponent.h>

namespace Degen
{
	namespace Input
	{
		bool cInput::mouse_on_window = false;
		double cInput::mouse_scroll_x_offset = 0;
		double cInput::mouse_scroll_y_offset = 0;

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
			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && mouse_on_window)
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

			if (glfwGetKey(window, GLFW_KEY_W))
			{
				glm::vec3 movement = View->target - View->position;
				movement.y = 0.f;
				movement = glm::normalize(movement);
				movement *= 5.f * dt;
				dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position += movement;

			}
			else if (glfwGetKey(window, GLFW_KEY_S))
			{
				glm::vec3 movement = View->target - View->position;
				movement.y = 0.f;
				movement = glm::normalize(movement);
				movement *= 5.f * dt;
				dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position -= movement;

			}

			if (glfwGetKey(window, GLFW_KEY_A))
			{
				glm::vec3 movement = glm::cross(View->target - View->position, View->up);
				movement.y = 0.f;
				movement = glm::normalize(movement);
				movement *= 5.f * dt;
				dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position -= movement;

			}
			else if (glfwGetKey(window, GLFW_KEY_D))
			{
				glm::vec3 movement = glm::cross(View->target - View->position, View->up);
				movement.y = 0.f;
				movement = glm::normalize(movement);
				movement *= 5.f * dt;
				dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position += movement;

			}

			if (glfwGetKey(window, GLFW_KEY_Q))
			{
				glm::vec3 movement(0.f);
				movement.y = 1.f;
				movement = glm::normalize(movement);
				movement *= 5.f * dt;
				dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position -= movement;

			}
			else if (glfwGetKey(window, GLFW_KEY_E))
			{
				glm::vec3 movement(0.f);
				movement.y = 1.f;
				movement = glm::normalize(movement);
				movement *= 5.f * dt;
				dynamic_cast<Component::Position*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::POSITION_COMPONENT))->position += movement;

			}

			if (glfwGetKey(window, GLFW_KEY_LEFT))
			{
				Component::Physics* phys = dynamic_cast<Component::Physics*>(Entity::cEntityManager::GetEntity("Left Paddle")->GetComponent(Component::PHYSICS_COMPONENT));

				Physics::iPaddleComponent* paddle = dynamic_cast<Physics::iPaddleComponent*>(phys->comp);

				paddle->Hit(glm::vec3(0, 0, 2000));
			}
			else
			{
				Component::Physics* phys = dynamic_cast<Component::Physics*>(Entity::cEntityManager::GetEntity("Left Paddle")->GetComponent(Component::PHYSICS_COMPONENT));

				Physics::iPaddleComponent* paddle = dynamic_cast<Physics::iPaddleComponent*>(phys->comp);

				paddle->Hit(glm::vec3(0, 0, -200));
				
			}
			
			if (glfwGetKey(window, GLFW_KEY_RIGHT))
			{
				Component::Physics* phys = dynamic_cast<Component::Physics*>(Entity::cEntityManager::GetEntity("Right Paddle")->GetComponent(Component::PHYSICS_COMPONENT));

				Physics::iPaddleComponent* paddle = dynamic_cast<Physics::iPaddleComponent*>(phys->comp);

				paddle->Hit(glm::vec3(0, 0, 2000));
			}
			else
			{
				Component::Physics* phys = dynamic_cast<Component::Physics*>(Entity::cEntityManager::GetEntity("Right Paddle")->GetComponent(Component::PHYSICS_COMPONENT));

				Physics::iPaddleComponent* paddle = dynamic_cast<Physics::iPaddleComponent*>(phys->comp);

				paddle->Hit(glm::vec3(0, 0, -200));

			}

			if (glfwGetKey(window, GLFW_KEY_SPACE))
			{
				Component::Physics* phys = dynamic_cast<Component::Physics*>(Entity::cEntityManager::GetEntity("Launcher")->GetComponent(Component::PHYSICS_COMPONENT));

				Physics::iLauncherComponent* launcher = dynamic_cast<Physics::iLauncherComponent*>(phys->comp);

				launcher->Pull(glm::vec3(0,0,-1));
				
				//Component::Animation* animation = dynamic_cast<Component::Animation*>(Entity::cEntityManager::GetEntity("forest guard")->GetComponent(Component::ANIMATION_COMPONENT));

				//animation->next_animation = "forest_guard@walk";
				////animation->play_through = true;
				//animation->next_animation_blend_time = 0.4;
			}
			else
			{
				Component::Physics* phys = dynamic_cast<Component::Physics*>(Entity::cEntityManager::GetEntity("Launcher")->GetComponent(Component::PHYSICS_COMPONENT));

				Physics::iLauncherComponent* launcher = dynamic_cast<Physics::iLauncherComponent*>(phys->comp);

				launcher->Release();
			}
		}
	}
}
