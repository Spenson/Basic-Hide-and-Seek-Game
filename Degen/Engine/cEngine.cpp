#include "cEngine.h"
#include <cassert>
#include <iostream>

#include "Render/cRenderer.h"
#include "Inputs/cInput.h"
#include "Camera/cCamera.h"
#include "AI/cIntelligence.h"

#include "FileReading/JsonHelpers.h"
#include "Load.h"

#include "VAO and Meshes/cVAOManager.h"
#include "Shaders/cShaderManager.h"
#include "FileReading/cModelLoader.h"
#include "Entity/cEntityManager.h"
#include "sView.h"
#include "BasicMotion/cBasicMotion.h"


namespace Degen
{
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	VAOAndModel::cVAOManager* VAOManager; // Manages objects pushed to gpu
	Shaders::cShaderManager* ShaderManager; // managers all shaders created
	FileReading::cModelLoader* ModelLoader; // manages files loded with assimp
	Entity::cEntityManager* EntityManager; // creats and cleans up entities
	sView* View;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}

	static void window_size_callback(GLFWwindow* window, int width, int height)
	{
		WINDOW_WIDTH = width;
		WINDOW_HEIGHT = height;
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
#ifdef  _DEBUG
		assert(glfwInit());
#else
		glfwInit();
#endif


		mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Degen engine...", 0, 0);
		if (!mWindow)
		{
			glfwTerminate();
			printf("Unable to create glfw window\n");
			return false;
		}

		glfwSetKeyCallback(mWindow, key_callback);
		glfwSetWindowSizeCallback(mWindow, window_size_callback);

		glfwMakeContextCurrent(mWindow);

		glfwSwapInterval(1);

		//Init glad
#ifdef  _DEBUG
		assert(gladLoadGL());
#else
		gladLoadGL();
#endif

		std::cout << glGetString(GL_VERSION) << std::endl;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Clear to grey
		glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

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

		if (jsonRoot["width"].isInt()) WINDOW_WIDTH = jsonRoot["width"].asInt();
		if (jsonRoot["height"].isInt()) WINDOW_HEIGHT = jsonRoot["height"].asInt();

		if (!InitGL())
		{
			printf("InitGL failed\n");
			return false;
		}

		VAOManager = new VAOAndModel::cVAOManager();
		ShaderManager = new Shaders::cShaderManager();
		ModelLoader = new FileReading::cModelLoader();
		EntityManager = new Entity::cEntityManager();
		View = new sView();

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

			ShaderManager->pGetShaderProgramFromFriendlyName(name)->LoadActiveUniforms();
			mRenderer = new Render::cRenderer(ShaderManager->pGetShaderProgramFromFriendlyName(name));
			mShaderName = name;
		}


		mCamera = new Camera::cCamera();
		mInput = new Input::cInput(mWindow);

		if(jsonRoot["ai_map"].isString())
		{
			mIntelligence = new AI::cIntelligence(jsonRoot["ai_map"].asString());
		}
		mBasicMotion = new BasicMotion::cBasicMotion();

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

		// TODO: TEXTURES

		if (!Load::LoadModels(jsonRoot["Models"], mShaderName))
		{
			printf("Could not load models.\n");
			return false;
		}

		// TODO: Animations
	
		if (!Load::LoadEntities(jsonRoot["Entities"]))
		{
			printf("Could not load entities.\n");
			return false;
		}

		for (auto* entity : Entity::cEntityManager::entities)
		{
			mRenderer->AddEntity(entity);
			mCamera->AddEntity(entity);
			mIntelligence->AddEntity(entity);
			mBasicMotion->AddEntity(entity);
		}

		return true;
	}

	bool cEngine::CleanUp()
	{
		return false;
	}

	void cEngine::Go()
	{
		double delta_time = 0;
		double previous_time = glfwGetTime();
		while (!glfwWindowShouldClose(mWindow))
		{
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//get window size
			glfwGetWindowSize(mWindow, &WINDOW_WIDTH, &WINDOW_HEIGHT);

			//set the view port
			//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

			double time = glfwGetTime();
			delta_time = glm::max(glm::min(time - previous_time, 0.05), 0.001); // min "simulate" 20fps, even if <20fps, max fps1000 (crazy but for error stuff)
			previous_time = time;

			mInput->Update(delta_time);
			mIntelligence->Update(delta_time);
			mBasicMotion->Update(delta_time);
			mCamera->Update(delta_time);
			mRenderer->Update(delta_time);


			//Swap buffers
			glfwSwapBuffers(mWindow);
			glfwPollEvents();
		}
	}
}

