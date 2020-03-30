#include "cInput.h"
#include "../Globals.h"
#include "../Component/Position.h"
#include "../Component/Camera.h"

namespace Degen
{
	namespace Input
	{
		bool cInput::mouse_on_window = false;

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

		cInput::cInput(GLFWwindow* window) : window(window)
		{
			glfwSetCursorEnterCallback(window, cursor_enter_callback);
		}
		void cInput::Update(double dt)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && mouse_on_window)
			{
				float diffX = x - last_x;
				float diffY = y - last_y;
				Component::Camera* camera = dynamic_cast<Component::Camera*>(Entity::cEntityManager::GetEntity(1000)->GetComponent(Component::CAMERA_COMPONENT));
				camera->pitch -= (diffY * 0.5f);
				camera->yaw -= (diffX * 0.5f);
			}
			last_x = x;
			last_y = y;
			
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
		}
	}
}
