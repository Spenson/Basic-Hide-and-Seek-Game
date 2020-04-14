#pragma once
#include <glm/mat4x2.hpp>
#include "../Shaders/cShaderManager.h"
#include "../FBO/cFBO.h"
#include "../Component/Render.h"
#include "../Component/Animation.h"
#include "../Entity/cEntity.h"

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
			void RenderObject(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, Component::Animation* animation, glm::mat4 transform, glm::mat4 parent_matrix = glm::mat4(1.0f));

			void AddEntity(Entity::cEntity* entity);

		private:
			bool LastDrawPass();
			bool PassLights();
			void EntitySinglePassSort();
			glm::vec3 GetPosition(Entity::cEntity* entity);
			glm::mat4 GetTransform(Entity::cEntity* entity);
			cFBO mFBO;
			Shaders::cShaderManager::cShaderProgram* mShaderProgram = nullptr;

			std::vector<Entity::cEntity*> mRenderEntities;
			std::vector<Entity::cEntity*> mLightEntities;
		};
	}
}
