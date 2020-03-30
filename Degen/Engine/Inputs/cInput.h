#pragma once
#include "../GL.h"


namespace Degen
{
	namespace Input
	{
		class cInput
		{
		public:
			cInput(GLFWwindow* window); 
			void Update(double dt);
			
			GLFWwindow* window;
			float last_x = 0, last_y = 0;
			static bool mouse_on_window;
		};
	}
}
