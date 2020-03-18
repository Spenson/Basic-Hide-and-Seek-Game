#pragma once

#include "GL.h"
#include "Globals.h"
#include <vector>
#include "Entities/iGameObject.h"

namespace Degen
{
	class cEngine
	{
	public:
		cEngine();
		virtual ~cEngine();

		bool Initialize();
		bool Load();
		bool CleanUp();
		void Go();
		
	private:
		GLFWwindow* mWindow;
		
		bool InitGL();
		
		// Master List For Clean-Up
		std::vector<Object::iGameObject*> entities;
	};

}
