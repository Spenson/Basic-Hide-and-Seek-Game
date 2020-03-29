#pragma once
#include <glm/mat4x2.hpp>
#include "../Shaders/cShaderManager.h"
#include "../FBO/cFBO.h"
#include "../Component/Render.h"

namespace Degen
{
	namespace Render
	{
		class cRenderer
		{
		public:
			cRenderer(Shaders::cShaderManager::cShaderProgram* shader);

			void Update(double dt = 0.);
			
			void Render(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, glm::mat4 transform, glm::mat4 parent_matrix = glm::mat4(1.0f));

			
			//bool SetShader(Shaders::cShaderManager::cShaderProgram* shader_program);
			//bool SingleRender(Object::iGameObject* object);
			//void SetCamera();
			//void RenderObject(Object::cRenderOnlyObject* object, Shaders::cShaderManager::cShaderProgram* shader_program, glm::mat4 parent_matrix = glm::mat4(1.f));

		private:
			cFBO mFBO;
			Shaders::cShaderManager::cShaderProgram* mShaderProgram = nullptr;
		};
	}
}
