#include "cEngine.h"
#include <cassert>
#include <iostream>

#include "FileReading/JsonHelpers.h"
#include "FileReading/cModelLoader.h"
#include "Shaders/cShaderManager.h"
#include "VAO and Meshes/cVAOManager.h"
#include "Managers.h"


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
		WINDOW_WIDTH = 800;
		WINDOW_HEIGHT = 600;

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

		mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Degen engine...", 0, 0);
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

		std::cout << glGetString(GL_VERSION) << std::endl;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Clear to grey
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		glLineWidth(3);

		return true;
	}

	bool cEngine::Initialize(std::string file)
	{
		Json::Value jsonRoot;
		{
			std::string loadingErrors;
			std::string fullFilePath = "./assets/config/" + file;
			if (FileReading::ReadJsonFile(fullFilePath, jsonRoot, loadingErrors))
			{
				std::cout << "init file " << file << " opened and read successfully" << std::endl;
			}
			else
			{
				std::cout << "couldn't read init file: " << file << "\n\tErrors: " << loadingErrors << std::endl;
				return false;
			}
		}

		if (jsonRoot["width"].isInt64()) WINDOW_WIDTH = jsonRoot["width"].asInt64();
		if (jsonRoot["height"].isInt64()) WINDOW_HEIGHT = jsonRoot["height"].asInt64();

		if (!InitGL())
		{
			printf("InitGL failed\n");
			return false;
		}

		VAOManager = new VAOAndModel::cVAOManager();
		ShaderManager = new Shaders::cShaderManager();
		ModelLoader = new FileReading::cModelLoader();
		

		return true;
	}

	bool cEngine::Load(std::string file)
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

