#pragma once

#include "GL.h"
#include <string>


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

	namespace Physics
	{
		class cPhysics;
	}

	namespace Game
	{
		class Pinball;
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
		Input::cInput* mInput;
		//AI::cIntelligence* mIntelligence;
		BasicMotion::cBasicMotion* mBasicMotion;
		Animation::cAnimator* mAnimator;
		Physics::cPhysics* mPhysics;
		Game::Pinball* mPinball;

		// Needed for loading
		std::string mShaderName;

		bool InitGL();

	};

}
