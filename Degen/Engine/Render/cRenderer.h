#pragma once
#include <glm/mat4x2.hpp>
#include "../Shaders/cShaderManager.h"
#include "../FBO/cFBO.h"
#include "../Component/Render.h"
#include "../Component/Animation.h"
#include "../Component/Animation_New.h"
#include "../Component/MultiTransform.h"
#include "../Entity/cEntity.h"
#include "../FBO/cSharedDepthFBO.h"



namespace Degen
{
	namespace Render
	{
		class cRenderer
		{
		public:
			cRenderer(Shaders::cShaderManager::cShaderProgram* shader);

			void Update(double dt = 0.);
			
			void RenderObject(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, glm::mat4 transform, glm::mat4 parent_matrix = glm::mat4(1.0f));
			void RenderObject(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, Component::MultiTransform* transforms, glm::mat4 parent_matrix = glm::mat4(1.0f));
			void RenderObject(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, Component::Animation* animation, glm::mat4 transform, glm::mat4 parent_matrix = glm::mat4(1.0f));
			void RenderObject(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, Component::Animation_New* animation, glm::mat4 transform, glm::mat4 parent_matrix = glm::mat4(1.0f));

			void AddEntity(Entity::cEntity* entity);

		private:
			void BindOutPutTextures();
			bool LastDrawPass();
			bool PassLights();
			void EntitySinglePassSort();
			glm::vec3 GetPosition(Entity::cEntity* entity);
			glm::mat4 GetTransform(Entity::cEntity* entity);
			cSharedDepthFBO mOpaquFBO;
			cSharedDepthFBO mTransparentFBO;
			Shaders::cShaderManager::cShaderProgram* mShaderProgram = nullptr;

			std::vector<Entity::cEntity*> mRenderEntities;
			std::vector<Entity::cEntity*> mLightEntities;
		};
	}
}
