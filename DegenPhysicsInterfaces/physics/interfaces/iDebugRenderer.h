#pragma once
#include <glm.hpp>

namespace Degen
{

namespace Physics
{
	class iDebugRenderer
	{
	public:
		virtual ~iDebugRenderer() {}
		virtual void DrawSphere(const glm::mat4& transform, float radius) = 0;
	};
}
}