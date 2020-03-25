#include "cRenderOnlyObject.h"
namespace Degen
{
	namespace Object
	{
		glm::mat4 cRenderOnlyObject::Transform()
		{
			return glm::mat4(1.f);
		}
	}
}