#pragma once

#include "GL.h"
#include "Globals.h"
#include <vector>
#include "Object/iGameObject.h"


namespace Degen
{
	namespace Render
	{
		class cRenderer;
	}
	class cEngine
	{
	public:
		cEngine();
		virtual ~cEngine();

		bool Initialize(std::string file);
		bool Load(std::string file);
		bool CleanUp();
		void Go();

	private:
		GLFWwindow* mWindow;
		Render::cRenderer* mRenderer;

		std::string mShaderName;

		bool InitGL();

		// Master List For Clean-Up? (Should be in a manager?)
		std::vector<Object::iGameObject*> entities;
	};

}
