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
			float mouse_position_x = 0, mouse_position_y = 0;
			static bool mouse_on_window;
			static double mouse_scroll_x_offset;
			static double mouse_scroll_y_offset;
		};
	}
}
