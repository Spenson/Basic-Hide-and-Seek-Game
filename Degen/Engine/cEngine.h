#pragma once

#include "GL.h"
#include <string>
#include <json/json.h>


namespace Degen
{
	// ... I should really make these systems
	namespace Render
	{
		class cRenderer;
	}
	namespace Camera
	{
		class cCamera;
	}
	namespace Input
	{
		class cInput;
	}
	namespace BasicMotion
	{
		class cBasicMotion;
	}
	namespace Animation
	{
		class cAnimator;
	}
	namespace Game
	{
		class HideAndSeek;
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

		void UpdateEntityLists();
		
		Game::HideAndSeek* mHideAndSeek;
	private:
		GLFWwindow* mWindow;
		Render::cRenderer* mRenderer;
		Camera::cCamera* mCamera;
		Input::cInput* mInput;
		BasicMotion::cBasicMotion* mBasicMotion;
		Animation::cAnimator* mAnimator;
		

		// Needed for loading
		std::string mShaderName;

		bool MakeUberShader(Json::Value& jsonRoot);
		
		bool InitGL();

	};

}
