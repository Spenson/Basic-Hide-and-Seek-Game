#pragma once
#include <glm.hpp>


namespace Degen
{
	struct sView
	{
		glm::vec3 position = glm::vec3(0.f,0.f,-10.f);
		glm::vec3 target = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);

		float fovy = 0.7f;
		float near_plane = 0.1f;
		float far_plane = 1000.f;
	};
}
