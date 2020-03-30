#pragma once

#include "GL.h"
#include <string>


namespace Degen
{
	namespace Render
	{
		class cRenderer;
	}
	namespace Camera
	{
		class cCamera;
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
		Camera::cCamera* mCamera;

		// Needed for loading
		std::string mShaderName;

		bool InitGL();

	};

}
