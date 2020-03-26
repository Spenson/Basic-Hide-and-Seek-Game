#include "cEngine.h"
#include <cassert>
#include <iostream>

#include "FileReading/JsonHelpers.h"
#include "FileReading/cModelLoader.h"
#include "Shaders/cShaderManager.h"
#include "VAO and Meshes/cVAOManager.h"
#include "Managers.h"
#include "Render/cRenderer.h"


namespace Degen
{
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	
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

		{
			Json::Value jsonShader;
			if (jsonRoot["shader"].isObject())jsonShader = jsonRoot["shader"];

			std::string name;
			if (jsonShader["name"].isString()) name = jsonRoot["name"].asString();

			Shaders::cShaderManager::cShader vertexShad;
			if (jsonShader["vert"].isString()) vertexShad.fileName = "assets/shaders/" + jsonShader["vert"].asString();

			Shaders::cShaderManager::cShader geoShader;
			if (jsonShader["geo"].isString()) geoShader.fileName = "assets/shaders/" + jsonShader["geo"].asString();

			Shaders::cShaderManager::cShader fragShader;
			if (jsonShader["frag"].isString()) fragShader.fileName = "assets/shaders/" + jsonShader["frag"].asString();


			if (vertexShad.fileName.empty() || fragShader.fileName.empty())
			{
				printf("vertex and fragment shaders required.\n");
				return false;
			}


			if (geoShader.fileName.empty())
			{
				if (!ShaderManager->createProgramFromFile(name, vertexShad, fragShader))
				{
					printf("Could not create shader\n");
					printf("%s\n", ShaderManager->getLastError().c_str());
					return false;
				}
			}
			else
			{
				if (!ShaderManager->createProgramFromFile(name, vertexShad, geoShader, fragShader))
				{
					printf("Could not create shader\n");
					printf("%s\n", ShaderManager->getLastError().c_str());
					return false;
				}
			}

			mRenderer = new Render::cRenderer(ShaderManager->pGetShaderProgramFromFriendlyName(name));
			mShaderName = name;
		}


		return true;
	}

	bool cEngine::Load(std::string file)
	{
		Json::Value jsonRoot;
		{
			std::string loadingErrors;
			std::string fullFilePath = "./assets/config/" + file;
			if (FileReading::ReadJsonFile(fullFilePath, jsonRoot, loadingErrors))
			{
				std::cout << "data file " << file << " opened and read successfully" << std::endl;
			}
			else
			{
				std::cout << "couldn't read data file: " << file << "\n\tErrors: " << loadingErrors << std::endl;
				return false;
			}
		}

		if (jsonRoot["Textures"].isArray())
		{
			const Json::Value& jsonTextures = jsonRoot["Textures"];
			for (size_t idx = 0; idx < jsonTextures.size(); idx++)
			{

			}
		}
		if (jsonRoot["Models"].isArray())
		{
			Json::Value jsonModels = &jsonRoot["Models"];
			std::string model_name;
			std::string model_file;
			bool is_basic;
			std::string error;
			for (unsigned int idx = 0; idx < jsonModels.size(); idx++)
			{
				Json::Value jsonCurModel = &jsonModels[idx];
				if (jsonCurModel["name"].isString()) model_name = jsonCurModel["name"].asString();
				if (jsonCurModel["file"].isString()) model_file = jsonCurModel["file"].asString();
				if (jsonCurModel["is_basic"].isBool()) is_basic = jsonCurModel["basic"].asBool();
				VAOAndModel::sModelDrawInfo* mdi = nullptr;

				if (is_basic)
				{
					mdi = ModelLoader->LoadBasicModel(model_file, model_name, error);
				}
				else
				{
					printf("Advanced models not implemented.\n");
				}
				
				if (!mdi)
				{
					printf("Model '%s' not loaded: %s\n", model_name.c_str(), error.c_str());
					continue;
				}

				if(!VAOManager->LoadModelDrawInfoIntoVAO(*mdi, ShaderManager->GetProgramIDFromFriendlyName(mShaderName)))
				{
					printf("Model '%s' not pushed to VAO.\n", model_name.c_str());
				}
			}

		}
		if (jsonRoot["Animations"].isArray())
		{
			// TODO
		}
		if (jsonRoot["Lights"].isArray())
		{
			// TODO
		}
		if (jsonRoot["Entities"].isArray())
		{
			Json::Value jsonObjects = &jsonRoot["Objects"];
			
			for (unsigned int idx = 0; idx < jsonObjects.size(); idx++)
			{
				Json::Value jsonCurObject = &jsonObjects[idx];
				
			}
		}

		return true;
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

