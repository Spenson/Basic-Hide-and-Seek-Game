#include "PinBall.h"
#include "../Globals.h"


namespace Degen
{
	namespace Game
	{
		Pinball::Pinball()
		{
		}
		void Pinball::Update(double dt)
		{
			TextRenderer->AddText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		}
	}
}