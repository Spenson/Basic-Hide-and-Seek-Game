#include "cEngine.h"
#include <cassert>

namespace Degen
{
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
	
	cEngine::cEngine()
	{
		WINDOW_WIDTH = 1280;
		WINDOW_HEIGHT = 720;

		if(!InitGL()) printf("InitGL failed\n");

		
	}

	cEngine::~cEngine()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	bool cEngine::InitGL()
	{
		// NOTE: assert not built into release
		assert(glfwInit());

		mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My ECS engine...", 0, 0);
		if (!mWindow)
		{
			glfwTerminate();
			printf("Unable to create glfw window\n");
			return false;
		}

		glfwSetKeyCallback(mWindow, key_callback);

		glfwMakeContextCurrent(mWindow);

		glfwSwapInterval(1);

		//Init glad
		assert(gladLoadGL());

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Clear to grey
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		glLineWidth(3);

		return true;
	}

	bool cEngine::Load()
	{
		return false;
	}

	bool cEngine::CleanUp()
	{
		return false;
	}

	void cEngine::Go()
	{
		while (!glfwWindowShouldClose(mWindow))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			//get window size
			glfwGetWindowSize(mWindow, &WINDOW_WIDTH, &WINDOW_HEIGHT);

			//set the view port
			glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

			//call this update
			//this->Update();

			//Swap buffers
			glfwSwapBuffers(mWindow);
			glfwPollEvents();
		}
	}
}

