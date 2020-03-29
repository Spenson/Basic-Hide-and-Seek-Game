#include "cRenderer.h"
#include <glad/glad.h>
#include "../Globals.h"
#include "../VAO and Meshes/sModelDrawInfo.h"
#include "../Component/Transform.h"

namespace Degen
{
	namespace Render
	{
		struct buffer
		{
			glm::mat4 matModel;
			glm::mat4 matModelInverseTranspose;

			glm::vec4 diffuseColour;
			glm::vec4 specularColour;
			glm::vec4 boolModifiers;
		};

		struct sLight
		{
			glm::vec4 position;
			glm::vec4 diffuse;
			glm::vec4 specular;
			glm::vec4 atten;
			glm::vec4 direction;
			glm::vec4 param1;
			glm::vec4 param2;
		};

		cRenderer::cRenderer(Shaders::cShaderManager::cShaderProgram* shader) : mFBO(), mShaderProgram(shader)
		{
			std::string error;
			if (!mFBO.init(WINDOW_WIDTH, WINDOW_HEIGHT, error))
			{
				printf("FBO init failed:\n\t%s\n", error.c_str());
				return;
			}
		}

		void cRenderer::Update(double dt)
		{
			if (WINDOW_WIDTH != mFBO.width || WINDOW_HEIGHT != mFBO.height)
			{
				std::string error;
				if (!mFBO.reset(WINDOW_WIDTH, WINDOW_HEIGHT, error))
				{
					printf("FBO reset failed:\n\t%s\n", error.c_str());
					return;
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, mFBO.ID);
			mFBO.clearBuffers(true, true);

			glUseProgram(mShaderProgram->ID);
			glUniform1i(mShaderProgram->GetUniformLocationID("passNumber"), 0);

			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);

			float ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
			glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);

			glm::mat4 perspective = glm::perspective(0.6f, ratio, 0.1f, 100.f);//mFOVY, ratio, mNearPlane, mFarPlane);
			glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, -10.f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));//scene->camera->Position(), scene->camera->Target(), scene->camera->UpVector());

			glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matView"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matProj"), 1, GL_FALSE, glm::value_ptr(perspective));

			for (auto* entity : Entity::cEntityManager::entities)
			{
				if (!entity->HasComponent(Component::RENDER_COMPONENT)) continue;

				Component::Render* rend = dynamic_cast<Component::Render*>(entity->GetComponent(Component::RENDER_COMPONENT));
				glm::mat4 tranform(1.f);
				if (entity->HasComponent(Component::TRANSFORM_COMPONENT))
				{
					tranform = dynamic_cast<Component::Transform*>(entity->GetComponent(Component::TRANSFORM_COMPONENT))->transform;
				}

				Render(mShaderProgram, rend, tranform);
			}

			
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glUniform1i(mShaderProgram->GetUniformLocationID("passNumber"), 1);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				view = glm::lookAt(glm::vec3(0.0f, 0.0f, -50.0f),
								   glm::vec3(0.0f, 0.0f, 0.0f),
								   glm::vec3(0.0f, 1.0f, 0.0f));

				glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matView"), 1, GL_FALSE, glm::value_ptr(view));

				glUniform1f(mShaderProgram->GetUniformLocationID("Width"), WINDOW_WIDTH);
				glUniform1f(mShaderProgram->GetUniformLocationID("Height"), WINDOW_HEIGHT);

				glUniform1f(mShaderProgram->GetUniformLocationID("blur"), 0.f);
				glUniform1f(mShaderProgram->GetUniformLocationID("nightvision"), 0.f);

				glm::mat4 matWorldCurrentGO = glm::scale(glm::mat4(1.f), glm::vec3(50.f));
				glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matModel"), 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));
				glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matWorldCurrentGO));
				glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matModelInverseTranspose"), 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

				glActiveTexture(GL_TEXTURE9);				// Texture Unit 0
				glBindTexture(GL_TEXTURE_2D, mFBO.colourTexture_ID);	// Texture now assoc with texture unit 0
				glActiveTexture(GL_TEXTURE10);				// Texture Unit 1
				glBindTexture(GL_TEXTURE_2D, mFBO.normalTexture_ID);	// Texture now assoc with texture unit 0
				glActiveTexture(GL_TEXTURE11);				// Texture Unit 2
				glBindTexture(GL_TEXTURE_2D, mFBO.positionTexture_ID);	// Texture now assoc with texture unit 0
				glActiveTexture(GL_TEXTURE12);				// Texture Unit 3
				glBindTexture(GL_TEXTURE_2D, mFBO.specularTexture_ID);	// Texture now assoc with texture unit 0

				// Tie the texture units to the samplers in the shader
				glUniform1i(mShaderProgram->GetUniformLocationID("textureColour"), 9);	// Texture unit 0
				glUniform1i(mShaderProgram->GetUniformLocationID("textureNormal"), 10);	// Texture unit 1
				glUniform1i(mShaderProgram->GetUniformLocationID("texturePosition"), 11);	// Texture unit 2
				glUniform1i(mShaderProgram->GetUniformLocationID("textureSpecular"), 12);	// Texture unit 3

				glUniform4f(mShaderProgram->GetUniformLocationID("diffuseColour"), 1.f, 1.f, 1.f, 1.f);	// *********
				glUniform4f(mShaderProgram->GetUniformLocationID("specularColour"), 1.f, 1.f, 1.f, 1.f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID

				glUniform4f(mShaderProgram->GetUniformLocationID("boolModifiers"), 0.f, 1.f, 0.f, 0.f);
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);
				glUniform1f(mShaderProgram->GetUniformLocationID("isSkinnedMesh"), (float)GL_FALSE);

				VAOAndModel::sModelDrawInfo drawInfo;
				//if (pTheVAOManager->FindDrawInfoByModelName("bunny", drawInfo))
				if (VAOManager->FindDrawInfoByModelName("quad", drawInfo))
				{
					glBindVertexArray(drawInfo.vao_id);
					glDrawElements(GL_TRIANGLES,
								   drawInfo.number_of_indices,
								   GL_UNSIGNED_INT,
								   0);
					glBindVertexArray(0);
				}
			}
		}

		void cRenderer::Render(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, glm::mat4 transform, glm::mat4 parent_matrix)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glCullFace(GL_BACK);

			glm::mat4 matWorldCurrentGO = parent_matrix * (transform * glm::scale(glm::mat4(1.f), rend_comp->scale));
			glUniformMatrix4fv(shader_program->GetUniformLocationID("matModel"), 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));

			glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matWorldCurrentGO));
			glUniformMatrix4fv(shader_program->GetUniformLocationID("matModelInverseTranspose"), 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

			glUniform4f(shader_program->GetUniformLocationID("diffuseColour"),
						rend_comp->diffuse_colour.r,
						rend_comp->diffuse_colour.g,
						rend_comp->diffuse_colour.b,
						rend_comp->diffuse_colour.a);

			glUniform4f(shader_program->GetUniformLocationID("specularColour"),
						rend_comp->specular_colour.r,
						rend_comp->specular_colour.g,
						rend_comp->specular_colour.b,
						rend_comp->specular_colour.a);	// Power

			float ignore_lighting = rend_comp->ignore_lighting;

			if (rend_comp->is_wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
				ignore_lighting = GL_TRUE;
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
			}
			float use_diffuse = true; //pCurrentObject->UseDiffuse();

			glUniform4f(shader_program->GetUniformLocationID("boolModifiers"),
						0.f,
						ignore_lighting,
						use_diffuse,
						0);

			glUniform1f(shader_program->GetUniformLocationID("isSkinnedMesh"), (float)GL_FALSE);

			VAOAndModel::sModelDrawInfo drawInfo;
			if (VAOManager->FindDrawInfoByModelName(rend_comp->mesh, drawInfo))
			{
				glBindVertexArray(drawInfo.vao_id);
				glDrawElements(GL_TRIANGLES,
							   drawInfo.number_of_indices,
							   GL_UNSIGNED_INT,
							   0);
				glBindVertexArray(0);
			}
		}

		//bool cRenderer::SingleRender(Object::iGameObject* object)
		//{
		//	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
		//	//glUniform1i(shaderProgram->getUniformLocID("passNumber"), 1);
		//
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//
		//	glDisable(GL_STENCIL_TEST);
		//	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		//
		//	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -50.0f),
		//								 glm::vec3(0.0f, 0.0f, 0.0f),
		//								 glm::vec3(0.0f, 1.0f, 0.0f));
		//	glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matView"), 1, GL_FALSE, glm::value_ptr(view));
		//
		//
		//	glUniform1f(mShaderProgram->GetUniformLocationID("Width"), WINDOW_WIDTH);
		//	glUniform1f(mShaderProgram->GetUniformLocationID("Height"), WINDOW_HEIGHT);
		//
		//
		//
		//	if (object->GetType() == Object::RENDER_ONLY_TYPE)
		//	{
		//		RenderObject(dynamic_cast<Object::cRenderOnlyObject*>(object), mShaderProgram);
		//		return true;
		//	}
		//	return false;
		//}

		//void cRenderer::RenderObject(Object::cRenderOnlyObject* object, Shaders::cShaderManager::cShaderProgram* shader_program, glm::mat4 parent_matrix)
		//{
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//	glCullFace(GL_BACK);
		//
		//	glEnable(GL_DEPTH_TEST);		// Turn ON depth test
		//	glDepthMask(GL_TRUE);			// Write to depth buffer
		//
		//	glm::mat4 matWorldCurrentGO = parent_matrix * (object->Transform() * glm::scale(glm::mat4(1.f), object->scale));
		//	glUniformMatrix4fv(shader_program->GetUniformLocationID("matModel"), 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));
		//
		//	glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matWorldCurrentGO));
		//	glUniformMatrix4fv(shader_program->GetUniformLocationID("matModelInverseTranspose"), 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));
		//
		//	glUniform4f(shader_program->GetUniformLocationID("diffuseColour"),
		//				object->diffuse_colour.r,
		//				object->diffuse_colour.g,
		//				object->diffuse_colour.b,
		//				object->diffuse_colour.a);
		//
		//	glUniform4f(shader_program->GetUniformLocationID("specularColour"),
		//				object->specular_colour.r,
		//				object->specular_colour.g,
		//				object->specular_colour.b,
		//				object->specular_colour.a);	// Power+
		//
		//
		//	float doNotLight = object->ignore_lighting;
		//
		//	if (object->is_wireframe)
		//	{
		//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
		//		doNotLight = GL_TRUE;
		//	}
		//	else
		//	{
		//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
		//	}
		//	float useDiffuse = true; //pCurrentObject->UseDiffuse();
		//
		//	glUniform4f(shader_program->GetUniformLocationID("boolModifiers"),
		//				0.f,
		//				doNotLight,
		//				useDiffuse,
		//				0);
		//
		//
		//	glUniform1f(shader_program->GetUniformLocationID("isSkinnedMesh"), (float)GL_FALSE);
		//
		//
		//	VAOAndModel::sModelDrawInfo drawInfo;
		//	if (VAOManager->FindDrawInfoByModelName(object->mesh, drawInfo))
		//	{
		//		glBindVertexArray(drawInfo.vao_id);
		//		glDrawElements(GL_TRIANGLES,
		//					   drawInfo.number_of_indices,
		//					   GL_UNSIGNED_INT,
		//					   0);
		//		glBindVertexArray(0);
		//	}
		//
		//}

	}
}

