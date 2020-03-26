#pragma once
#include <glm/mat4x2.hpp>
#include "../Object/cRenderOnlyObject.h"
#include "../Shaders/cShaderManager.h"
#include "../VAO and Meshes/cVAOManager.h"
#include "../FBO/cFBO.h"

namespace Degen
{
	namespace Render
	{
		class cRenderer
		{
		public:
			cRenderer(Shaders::cShaderManager::cShaderProgram* shader);

			void Update(double dt = 0.);
			

			bool SetShader(Shaders::cShaderManager::cShaderProgram* shader_program);
			bool SingleRender(Object::iGameObject* object);
			void SetCamera();
			void RenderObject(Object::cRenderOnlyObject* object, Shaders::cShaderManager::cShaderProgram* shader_program, glm::mat4 parent_matrix = glm::mat4(1.f));

		private:
			cFBO mFBO;
			Shaders::cShaderManager::cShaderProgram* mShaderProgram = nullptr;
		};
	}
}
