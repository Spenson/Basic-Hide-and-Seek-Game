#include "cRenderer.h"
#include <glad/glad.h>
#include <algorithm>
#include "../Globals.h"
#include "../VAO and Meshes/sModelDrawInfo.h"
#include "../Component/Transform.h"
#include "../Component/Position.h"
#include "../Component/Rotation.h"
#include "../Component/Light.h"
#include "../LightHelper/cLightHelper.h"
#include "../Component/Animation.h"

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

		const int NUMBEROFLIGHTS = 100;
		GLuint light_buffer = -1;
		struct sLight
		{
			glm::vec4 position;
			glm::vec4 diffuse;
			glm::vec4 specular;
			glm::vec4 atten;
			glm::vec4 direction;
			glm::vec4 param1;
			glm::vec4 param2;
		} Lights[NUMBEROFLIGHTS];

		cRenderer::cRenderer(Shaders::cShaderManager::cShaderProgram* shader) : mOpaquFBO(), mTransparentFBO(), mShaderProgram(shader)
		{
			std::string error;
			if (!mOpaquFBO.init(WINDOW_WIDTH, WINDOW_HEIGHT, error, true))
			{
				printf("FBO init failed:\n\t%s\n", error.c_str());
				return;
			}
			if (!mTransparentFBO.init(WINDOW_WIDTH, WINDOW_HEIGHT, error, false))
			{
				printf("FBO init failed:\n\t%s\n", error.c_str());
				return;
			}

			glUseProgram(shader->ID);

			GLuint bindingPoint = 1;
			GLuint block_index = glGetUniformBlockIndex(shader->ID, "LightBlock");
			glUniformBlockBinding(shader->ID, block_index, bindingPoint);

			//GLuint buffer = 0;
			glGenBuffers(1, &light_buffer);
			glBindBuffer(GL_UNIFORM_BUFFER, light_buffer);

			glBufferData(GL_UNIFORM_BUFFER, sizeof(sLight) * NUMBEROFLIGHTS, Lights, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, light_buffer);

			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			// doing because of ones below
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUniform1i(mShaderProgram->GetUniformLocationID("texture00"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUniform1i(mShaderProgram->GetUniformLocationID("texture01"), 0);
			//must be bound before first render of cube map texture sampling "texture(skybox00, vOut.normal.xyz)" will cause crash
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glUniform1i(mShaderProgram->GetUniformLocationID("skybox00"), 0);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glUniform1i(mShaderProgram->GetUniformLocationID("skybox01"), 0);

			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUniform1i(mShaderProgram->GetUniformLocationID("bump_map"), 0);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUniform1i(mShaderProgram->GetUniformLocationID("specular_map"), 0);

			BindOutPutTextures();

		}

		void cRenderer::Update(double dt)
		{
			if (WINDOW_WIDTH != mOpaquFBO.width || WINDOW_HEIGHT != mOpaquFBO.height)
			{
				std::string error;
				if (!mOpaquFBO.reset(WINDOW_WIDTH, WINDOW_HEIGHT, error, true))
				{
					printf("FBO reset failed:\n\t%s\n", error.c_str());
					return;
				}

				if (!mTransparentFBO.reset(WINDOW_WIDTH, WINDOW_HEIGHT, error, false))
				{
					printf("FBO reset failed:\n\t%s\n", error.c_str());
					return;
				}

				// REBIND TO NEW FBOs!
				BindOutPutTextures();
			}

			
			glUniform1f(mShaderProgram->GetUniformLocationID("Ambient"), 0.05);

			EntitySinglePassSort();

			glUseProgram(mShaderProgram->ID);
			glUniform1i(mShaderProgram->GetUniformLocationID("passNumber"), 0);

			glBindFramebuffer(GL_FRAMEBUFFER, mOpaquFBO.ID);
			mOpaquFBO.clearBuffers(true, true);

			glUniform1f(mShaderProgram->GetUniformLocationID("Width"), (float)WINDOW_WIDTH);
			glUniform1f(mShaderProgram->GetUniformLocationID("Height"), (float)WINDOW_HEIGHT);

			PassLights();

			glEnable(GL_DEPTH);
			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glEnable(GL_CULL_FACE);

			float ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
			glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 perspective = glm::perspective(View->fovy, ratio, View->near_plane, View->far_plane);
			glm::mat4 view = glm::lookAt(View->position, View->target, View->up);

			glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matView"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matProj"), 1, GL_FALSE, glm::value_ptr(perspective));

			std::vector<Entity::cEntity*> alpha_objects;

			for (auto* entity : mRenderEntities)
			{
				std::vector<Component::iComponent*> rends = entity->GetComponents(Component::RENDER_COMPONENT);
				
				Component::Animation* animation = dynamic_cast<Component::Animation*>(entity->GetComponent(Component::ANIMATION_COMPONENT));
				Component::Animation_New* animation_new = dynamic_cast<Component::Animation_New*>(entity->GetComponent(Component::ANIMATION_NEW_COMPONENT));

				Component::MultiTransform* mt = dynamic_cast<Component::MultiTransform*>(entity->GetComponent(Component::MULTI_TRANSFORM_COMPONENT));
				if (mt)
				{
					for (unsigned i = 0; i < rends.size(); i++)
					{
						Component::Render* rend = dynamic_cast<Component::Render*>(rends[i]);
						RenderObject(mShaderProgram, rend, mt);
					}

					continue;
				}
				
				glm::mat4 tranform = GetTransform(entity);
				bool in_alpha = false;
				for (unsigned i = 0; i < rends.size(); i++)
				{
					Component::Render* rend = dynamic_cast<Component::Render*>(rends[i]);

					if (rend->diffuse_colour.a < 1.f && !in_alpha)
					{
						alpha_objects.push_back(entity);
						in_alpha = true;
						continue;
					}

					
					if (animation)
					{
						RenderObject(mShaderProgram, rend, animation, tranform);
					}
					else if (animation_new)
					{
						RenderObject(mShaderProgram, rend, animation_new, tranform);
					}
					else
					{
						RenderObject(mShaderProgram, rend, tranform);
					}
				}
			}


			glBindFramebuffer(GL_FRAMEBUFFER, mTransparentFBO.ID);
			mTransparentFBO.clearBuffers(true, false);
			glUniform1i(mShaderProgram->GetUniformLocationID("passNumber"), 1);
			glClear(GL_COLOR_BUFFER_BIT);
			for (auto* entity : alpha_objects)
			{
				std::vector<Component::iComponent*> rends = entity->GetComponents(Component::RENDER_COMPONENT);

				Component::Animation* animation = dynamic_cast<Component::Animation*>(entity->GetComponent(Component::ANIMATION_COMPONENT));
				Component::Animation_New* animation_new = dynamic_cast<Component::Animation_New*>(entity->GetComponent(Component::ANIMATION_NEW_COMPONENT));

				Component::MultiTransform* mt = dynamic_cast<Component::MultiTransform*>(entity->GetComponent(Component::MULTI_TRANSFORM_COMPONENT));
				if (mt)
				{
					for (unsigned i = 0; i < rends.size(); i++)
					{
						Component::Render* rend = dynamic_cast<Component::Render*>(rends[i]);
						RenderObject(mShaderProgram, rend, mt);
					}

					continue;
				}

				glm::mat4 tranform = GetTransform(entity);
				for (unsigned i = 0; i < rends.size(); i++)
				{
					Component::Render* rend = dynamic_cast<Component::Render*>(rends[i]);

					if (rend->diffuse_colour.a == 1.f) continue;

					
					if (animation)
					{
						RenderObject(mShaderProgram, rend, animation, tranform);
					}
					else if (animation_new)
					{
						RenderObject(mShaderProgram, rend, animation_new, tranform);
					}
					else
					{
						RenderObject(mShaderProgram, rend, tranform);
					}
				}
			}

			if (!LastDrawPass())
			{
				printf("could not render scene.\n");
			}

		}

		void cRenderer::RenderObject(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, glm::mat4 transform, glm::mat4 parent_matrix)
		{
			if (rend_comp->cull_face_back) { glEnable(GL_CULL_FACE); }
			else { glDisable(GL_CULL_FACE); }
			glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glCullFace(GL_BACK);

			glm::mat4 matWorldCurrentGO = (transform * glm::scale(glm::mat4(1.f), rend_comp->scale));
			glUniformMatrix4fv(shader_program->GetUniformLocationID("matModel"), 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));

			glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matWorldCurrentGO));
			glUniformMatrix4fv(shader_program->GetUniformLocationID("matInvTrans"), 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));


			if (rend_comp->is_cubemap_textures)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager->getTextureIDFromName(rend_comp->texture1));
				glUniform1i(shader_program->GetUniformLocationID("skybox00"), 2);
				if (!rend_comp->texture2.empty())
				{
					glActiveTexture(GL_TEXTURE3);
					glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager->getTextureIDFromName(rend_comp->texture2));
					glUniform1i(shader_program->GetUniformLocationID("skybox01"), 3);
				}
				glUniform1f(shader_program->GetUniformLocationID("use_bump_map"), (float)GL_FALSE);
				glUniform1f(shader_program->GetUniformLocationID("use_specular_map"), (float)GL_FALSE);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->texture1));
				glUniform1i(shader_program->GetUniformLocationID("texture00"), 0);

				if (!rend_comp->texture2.empty())
				{
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->texture2));
					glUniform1i(shader_program->GetUniformLocationID("texture01"), 1);
				}

				if (rend_comp->use_bump_map)
				{
					glActiveTexture(GL_TEXTURE4);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->bump_map));
					glUniform1i(shader_program->GetUniformLocationID("bump_map"), 4);

					glUniform1f(shader_program->GetUniformLocationID("use_bump_map"), (float)GL_TRUE);
				}
				else
				{
					glUniform1f(shader_program->GetUniformLocationID("use_bump_map"), (float)GL_FALSE);
				}

				if (rend_comp->use_specular_map)
				{
					glActiveTexture(GL_TEXTURE5);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->specular_map));
					glUniform1i(shader_program->GetUniformLocationID("specular_map"), 5);

					glUniform1f(shader_program->GetUniformLocationID("use_specular_map"), (float)GL_TRUE);
				}
				else
				{
					glUniform1f(shader_program->GetUniformLocationID("use_specular_map"), (float)GL_FALSE);
				}

			}



			glUniform4f(shader_program->GetUniformLocationID("colour_ratios"),
						rend_comp->diffuse_amount,
						rend_comp->texture1_amount,
						rend_comp->texture2_amount,
						0.f);

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
			//float use_diffuse = true; //pCurrentObject->UseDiffuse();

			glUniform4f(shader_program->GetUniformLocationID("modifiers"),
						rend_comp->is_cubemap_textures ? (float)GL_TRUE : (float)GL_FALSE,
						ignore_lighting,
						0.f,
						0.f);

			glUniform1f(shader_program->GetUniformLocationID("isSkinnedMesh"), (float)GL_FALSE);

			VAOAndModel::sModelDrawInfo drawInfo;
			if (VAOManager->FindDrawInfoByModelName(rend_comp->mesh, drawInfo))
			{
				glBindVertexArray(drawInfo.vao_id);
				//*
				glPatchParameteri(GL_PATCH_VERTICES, 3);
				glDrawElements(GL_PATCHES,
							   drawInfo.number_of_indices,
							   GL_UNSIGNED_INT,
							   0);
				//*/
				/*
				glDrawElements(GL_TRIANGLES,
							   drawInfo.number_of_indices,
							   GL_UNSIGNED_INT,
							   0);
				//*/
				glBindVertexArray(0);
			}
		}

		void cRenderer::RenderObject(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, Component::MultiTransform* transforms, glm::mat4 parent_matrix)
		{
			if (rend_comp->cull_face_back) { glEnable(GL_CULL_FACE); }
			else { glDisable(GL_CULL_FACE); }
			glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glCullFace(GL_BACK);


			if (rend_comp->is_cubemap_textures)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager->getTextureIDFromName(rend_comp->texture1));
				glUniform1i(shader_program->GetUniformLocationID("skybox00"), 2);
				if (!rend_comp->texture2.empty())
				{
					glActiveTexture(GL_TEXTURE3);
					glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager->getTextureIDFromName(rend_comp->texture2));
					glUniform1i(shader_program->GetUniformLocationID("skybox01"), 3);
				}
				glUniform1f(shader_program->GetUniformLocationID("use_bump_map"), (float)GL_FALSE);
				glUniform1f(shader_program->GetUniformLocationID("use_specular_map"), (float)GL_FALSE);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->texture1));
				glUniform1i(shader_program->GetUniformLocationID("texture00"), 0);

				if (!rend_comp->texture2.empty())
				{
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->texture2));
					glUniform1i(shader_program->GetUniformLocationID("texture01"), 1);
				}

				if (rend_comp->use_bump_map)
				{
					glActiveTexture(GL_TEXTURE4);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->bump_map));
					glUniform1i(shader_program->GetUniformLocationID("bump_map"), 4);

					glUniform1f(shader_program->GetUniformLocationID("use_bump_map"), (float)GL_TRUE);
				}
				else
				{
					glUniform1f(shader_program->GetUniformLocationID("use_bump_map"), (float)GL_FALSE);
				}

				if (rend_comp->use_specular_map)
				{
					glActiveTexture(GL_TEXTURE5);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->specular_map));
					glUniform1i(shader_program->GetUniformLocationID("specular_map"), 5);

					glUniform1f(shader_program->GetUniformLocationID("use_specular_map"), (float)GL_TRUE);
				}
				else
				{
					glUniform1f(shader_program->GetUniformLocationID("use_specular_map"), (float)GL_FALSE);
				}

			}


			glUniform4f(shader_program->GetUniformLocationID("colour_ratios"),
						rend_comp->diffuse_amount,
						rend_comp->texture1_amount,
						rend_comp->texture2_amount,
						0.f);

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
			//float use_diffuse = true; //pCurrentObject->UseDiffuse();

			glUniform4f(shader_program->GetUniformLocationID("modifiers"),
						rend_comp->is_cubemap_textures ? (float)GL_TRUE : (float)GL_FALSE,
						ignore_lighting,
						0.f,
						0.f);

			glUniform1f(shader_program->GetUniformLocationID("isSkinnedMesh"), (float)GL_FALSE);


			
			for (auto transform : transforms->transforms)
			{
				glm::mat4 matWorldCurrentGO = (transform * glm::scale(glm::mat4(1.f), rend_comp->scale));
				glUniformMatrix4fv(shader_program->GetUniformLocationID("matModel"), 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));

				glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matWorldCurrentGO));
				glUniformMatrix4fv(shader_program->GetUniformLocationID("matInvTrans"), 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));


				VAOAndModel::sModelDrawInfo drawInfo;
				if (VAOManager->FindDrawInfoByModelName(rend_comp->mesh, drawInfo))
				{
					glBindVertexArray(drawInfo.vao_id);
					//*
					glPatchParameteri(GL_PATCH_VERTICES, 3);
					glDrawElements(GL_PATCHES,
								   drawInfo.number_of_indices,
								   GL_UNSIGNED_INT,
								   0);
					//*/
					/*
					glDrawElements(GL_TRIANGLES,
								   drawInfo.number_of_indices,
								   GL_UNSIGNED_INT,
								   0);
					//*/
					glBindVertexArray(0);
				}

			}


		}

		void cRenderer::RenderObject(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, Component::Animation* animation, glm::mat4 transform, glm::mat4 parent_matrix)
		{
			if (rend_comp->cull_face_back) { glEnable(GL_CULL_FACE); }
			else { glDisable(GL_CULL_FACE); }

			glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glCullFace(GL_BACK);

			glm::mat4 matWorldCurrentGO = (transform * glm::scale(glm::mat4(1.f), rend_comp->scale));
			glUniformMatrix4fv(shader_program->GetUniformLocationID("matModel"), 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));

			glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matWorldCurrentGO));
			glUniformMatrix4fv(shader_program->GetUniformLocationID("matInvTrans"), 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));


			if (rend_comp->is_cubemap_textures)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager->getTextureIDFromName(rend_comp->texture1));
				glUniform1i(shader_program->GetUniformLocationID("skybox00"), 2);
				if (!rend_comp->texture2.empty())
				{
					glActiveTexture(GL_TEXTURE3);
					glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager->getTextureIDFromName(rend_comp->texture2));
					glUniform1i(shader_program->GetUniformLocationID("skybox01"), 3);
				}
			}
			else
			{
				GLuint texSamp0_UL = TextureManager->getTextureIDFromName(rend_comp->texture1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texSamp0_UL);
				glUniform1i(shader_program->GetUniformLocationID("texture00"), 0);

				if (!rend_comp->texture2.empty())
				{
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->texture2));
					glUniform1i(shader_program->GetUniformLocationID("texture01"), 1);
				}

				if (rend_comp->use_bump_map)
				{
					glActiveTexture(GL_TEXTURE4);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->bump_map));
					glUniform1i(shader_program->GetUniformLocationID("bump_map"), 4);

					glUniform1f(shader_program->GetUniformLocationID("use_bump_map"), (float)GL_TRUE);
				}
				else
				{
					glUniform1f(shader_program->GetUniformLocationID("use_bump_map"), (float)GL_FALSE);
				}

				if (rend_comp->use_specular_map)
				{
					glActiveTexture(GL_TEXTURE5);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->specular_map));
					glUniform1i(shader_program->GetUniformLocationID("specular_map"), 5);

					glUniform1f(shader_program->GetUniformLocationID("use_specular_map"), (float)GL_TRUE);
				}
				else
				{
					glUniform1f(shader_program->GetUniformLocationID("use_specular_map"), (float)GL_FALSE);
				}
			}
			glUniform4f(shader_program->GetUniformLocationID("colour_ratios"),
						rend_comp->diffuse_amount,
						rend_comp->texture1_amount,
						rend_comp->texture2_amount,
						0.f);

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
			//float use_diffuse = true; //pCurrentObject->UseDiffuse();

			glUniform4f(shader_program->GetUniformLocationID("modifiers"),
						rend_comp->is_cubemap_textures ? (float)GL_TRUE : (float)GL_FALSE,
						ignore_lighting,
						0.f,
						0.f);

			if (!animation->bone_transforms.empty())
			{
				glUniform1f(shader_program->GetUniformLocationID("isSkinnedMesh"), (float)GL_TRUE);
				glUniformMatrix4fv(shader_program->GetUniformLocationID("animation[0]"), animation->bone_transforms.size(),
								   GL_FALSE,
								   glm::value_ptr(animation->bone_transforms[0]));
			}
			else
			{
				glUniform1f(shader_program->GetUniformLocationID("isSkinnedMesh"), (float)GL_FALSE);
			}
			VAOAndModel::sModelDrawInfo drawInfo;
			if (VAOManager->FindDrawInfoByModelName(rend_comp->mesh, drawInfo))
			{
				glBindVertexArray(drawInfo.vao_id);
				//*
				glPatchParameteri(GL_PATCH_VERTICES, 3);
				glDrawElements(GL_PATCHES,
							   drawInfo.number_of_indices,
							   GL_UNSIGNED_INT,
							   0);
				//*/
				/*
				glDrawElements(GL_TRIANGLES,
							   drawInfo.number_of_indices,
							   GL_UNSIGNED_INT,
							   0);
				//*/
				glBindVertexArray(0);
			}
		}

		void cRenderer::RenderObject(Shaders::cShaderManager::cShaderProgram* shader_program, Component::Render* rend_comp, Component::Animation_New* animation, glm::mat4 transform, glm::mat4 parent_matrix)
		{
			if (rend_comp->cull_face_back) { glEnable(GL_CULL_FACE); }
			else { glDisable(GL_CULL_FACE); }

			glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glCullFace(GL_BACK);

			glm::mat4 matWorldCurrentGO = (transform * glm::scale(glm::mat4(1.f), rend_comp->scale));
			glUniformMatrix4fv(shader_program->GetUniformLocationID("matModel"), 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));

			glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matWorldCurrentGO));
			glUniformMatrix4fv(shader_program->GetUniformLocationID("matInvTrans"), 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));


			if (rend_comp->is_cubemap_textures)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager->getTextureIDFromName(rend_comp->texture1));
				glUniform1i(shader_program->GetUniformLocationID("skybox00"), 2);
				if (!rend_comp->texture2.empty())
				{
					glActiveTexture(GL_TEXTURE3);
					glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager->getTextureIDFromName(rend_comp->texture2));
					glUniform1i(shader_program->GetUniformLocationID("skybox01"), 3);
				}
			}
			else
			{
				GLuint texSamp0_UL = TextureManager->getTextureIDFromName(rend_comp->texture1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texSamp0_UL);
				glUniform1i(shader_program->GetUniformLocationID("texture00"), 0);

				if (!rend_comp->texture2.empty())
				{
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->texture2));
					glUniform1i(shader_program->GetUniformLocationID("texture01"), 1);
				}

				if (rend_comp->use_bump_map)
				{
					glActiveTexture(GL_TEXTURE4);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->bump_map));
					glUniform1i(shader_program->GetUniformLocationID("bump_map"), 4);

					glUniform1f(shader_program->GetUniformLocationID("use_bump_map"), (float)GL_TRUE);
				}
				else
				{
					glUniform1f(shader_program->GetUniformLocationID("use_bump_map"), (float)GL_FALSE);
				}

				if (rend_comp->use_specular_map)
				{
					glActiveTexture(GL_TEXTURE5);
					glBindTexture(GL_TEXTURE_2D, TextureManager->getTextureIDFromName(rend_comp->specular_map));
					glUniform1i(shader_program->GetUniformLocationID("specular_map"), 5);

					glUniform1f(shader_program->GetUniformLocationID("use_specular_map"), (float)GL_TRUE);
				}
				else
				{
					glUniform1f(shader_program->GetUniformLocationID("use_specular_map"), (float)GL_FALSE);
				}
			}
			glUniform4f(shader_program->GetUniformLocationID("colour_ratios"),
						rend_comp->diffuse_amount,
						rend_comp->texture1_amount,
						rend_comp->texture2_amount,
						0.f);

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
			//float use_diffuse = true; //pCurrentObject->UseDiffuse();

			glUniform4f(shader_program->GetUniformLocationID("modifiers"),
						rend_comp->is_cubemap_textures ? (float)GL_TRUE : (float)GL_FALSE,
						ignore_lighting,
						0.f,
						0.f);

			if (!animation->bone_transforms.empty())
			{
				glUniform1f(shader_program->GetUniformLocationID("isSkinnedMesh"), (float)GL_TRUE);
				glUniformMatrix4fv(shader_program->GetUniformLocationID("animation[0]"), animation->bone_transforms.size(),
								   GL_FALSE,
								   glm::value_ptr(animation->bone_transforms[0]));
			}
			else
			{
				glUniform1f(shader_program->GetUniformLocationID("isSkinnedMesh"), (float)GL_FALSE);
			}
			VAOAndModel::sModelDrawInfo drawInfo;
			if (VAOManager->FindDrawInfoByModelName(rend_comp->mesh, drawInfo))
			{
				glBindVertexArray(drawInfo.vao_id);
				//*
				glPatchParameteri(GL_PATCH_VERTICES, 3);
				glDrawElements(GL_PATCHES,
							   drawInfo.number_of_indices,
							   GL_UNSIGNED_INT,
							   0);
				//*/
				/*
				glDrawElements(GL_TRIANGLES,
							   drawInfo.number_of_indices,
							   GL_UNSIGNED_INT,
							   0);
				//*/
				glBindVertexArray(0);
			}
		}

		void cRenderer::AddEntity(Entity::cEntity* entity)
		{
			if (entity->HasComponent(Component::RENDER_COMPONENT) &&
				(entity->HasComponent(Component::TRANSFORM_COMPONENT) || entity->HasComponent(Component::POSITION_COMPONENT) || entity->HasComponent(Component::MULTI_TRANSFORM_COMPONENT)))
			{
				if (std::find(mRenderEntities.begin(), mRenderEntities.end(), entity) == mRenderEntities.end())
				{
					mRenderEntities.push_back(entity);
				}
			}

			if (entity->HasComponent(Component::LIGHT_COMPONENT) &&
				(entity->HasComponent(Component::TRANSFORM_COMPONENT) || entity->HasComponent(Component::POSITION_COMPONENT)))
			{
				if (std::find(mLightEntities.begin(), mLightEntities.end(), entity) == mLightEntities.end())
				{
					mLightEntities.push_back(entity);
				}
			}
		}

		void cRenderer::BindOutPutTextures()
		{
			//Start at 20 because spacing
			glActiveTexture(GL_TEXTURE20);
			glBindTexture(GL_TEXTURE_2D, mOpaquFBO.texture_colour_ID);
			glActiveTexture(GL_TEXTURE21);
			glBindTexture(GL_TEXTURE_2D, mOpaquFBO.texture_normal_ID);
			glActiveTexture(GL_TEXTURE22);
			glBindTexture(GL_TEXTURE_2D, mOpaquFBO.texture_position_ID);
			glActiveTexture(GL_TEXTURE23);
			glBindTexture(GL_TEXTURE_2D, mOpaquFBO.texture_specular_ID);

			glActiveTexture(GL_TEXTURE24);
			glBindTexture(GL_TEXTURE_2D, mTransparentFBO.texture_colour_ID);
			glActiveTexture(GL_TEXTURE25);
			glBindTexture(GL_TEXTURE_2D, mTransparentFBO.texture_normal_ID);
			glActiveTexture(GL_TEXTURE26);
			glBindTexture(GL_TEXTURE_2D, mTransparentFBO.texture_position_ID);
			glActiveTexture(GL_TEXTURE27);
			glBindTexture(GL_TEXTURE_2D, mTransparentFBO.texture_specular_ID);

			// Tie the texture1 units to the samplers in the shader
			glUniform1i(mShaderProgram->GetUniformLocationID("textureColour"), 20);	// Texture unit 0
			glUniform1i(mShaderProgram->GetUniformLocationID("textureNormal"), 21);	// Texture unit 1
			glUniform1i(mShaderProgram->GetUniformLocationID("texturePosition"), 22);	// Texture unit 2
			glUniform1i(mShaderProgram->GetUniformLocationID("textureSpecular"), 23);	// Texture unit 3

			// Tie the texture1 units to the samplers in the shader
			glUniform1i(mShaderProgram->GetUniformLocationID("textureAlphaColour"), 24);	// Texture unit 0
			glUniform1i(mShaderProgram->GetUniformLocationID("textureAlphaNormal"), 25);	// Texture unit 1
			glUniform1i(mShaderProgram->GetUniformLocationID("textureAlphaPosition"), 26);	// Texture unit 2
			glUniform1i(mShaderProgram->GetUniformLocationID("textureAlphaSpecular"), 27);	// Texture unit 3

		}

		bool cRenderer::LastDrawPass()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUniform1i(mShaderProgram->GetUniformLocationID("passNumber"), 2);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glUniform4f(mShaderProgram->GetUniformLocationID("eyeLocation"),
						View->position.x,
						View->position.y,
						View->position.z,
						1.f);

			glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -50.0f),
										 glm::vec3(0.0f, 0.0f, 0.0f),
										 glm::vec3(0.0f, 1.0f, 0.0f));

			glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matView"), 1, GL_FALSE, glm::value_ptr(view));



			glm::mat4 matWorldCurrentGO = glm::scale(glm::mat4(1.f), glm::vec3(50.f));
			glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matModel"), 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));
			glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matWorldCurrentGO));
			glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matInvTrans"), 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));


			glUniform4f(mShaderProgram->GetUniformLocationID("diffuseColour"), 1.f, 1.f, 1.f, 1.f);	// *********
			glUniform4f(mShaderProgram->GetUniformLocationID("specularColour"), 1.f, 1.f, 1.f, 1.f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID

			glUniform4f(mShaderProgram->GetUniformLocationID("modifiers"), 0.f, 1.f, 0.f, 0.f);
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			glUniform1f(mShaderProgram->GetUniformLocationID("isSkinnedMesh"), (float)GL_FALSE);

			VAOAndModel::sModelDrawInfo drawInfo;
			//if (pTheVAOManager->FindDrawInfoByModelName("bunny", drawInfo))

			bool rendered = false;

			if (VAOManager->FindDrawInfoByModelName("quad", drawInfo))
			{
				glBindVertexArray(drawInfo.vao_id);
				//*
				glPatchParameteri(GL_PATCH_VERTICES, 3);
				glDrawElements(GL_PATCHES,
							   drawInfo.number_of_indices,
							   GL_UNSIGNED_INT,
							   0);
				//*/
				/*
				glDrawElements(GL_TRIANGLES,
							   drawInfo.number_of_indices,
							   GL_UNSIGNED_INT,
							   0);
				//*/
				glBindVertexArray(0);
				rendered = true;
			}


			return rendered;
		}

		bool cRenderer::PassLights()
		{
			unsigned light_idx = 0;
			for (auto* entity : mLightEntities)
			{
				std::vector<Component::iComponent*> these_are_lights = entity->GetComponents(Component::LIGHT_COMPONENT);
				glm::mat4 transform = GetTransform(entity);
				float distanceCutOff = FLT_MAX;
				for (auto* light_comp : these_are_lights)
				{
					auto* light = dynamic_cast<Component::Light*>(light_comp);

					Lights[light_idx].position = transform * glm::vec4(light->position, 1.f);
					Lights[light_idx].diffuse = light->diffuse;
					Lights[light_idx].specular = light->specular;

					distanceCutOff = light->cutoff;
					if (distanceCutOff == 0.f && light->on)
					{
						distanceCutOff = cLightHelper::calcApproxDistFromAtten(0.01f,
																			   0.001f,
																			   FLT_MAX,
																			   light->const_atten,
																			   light->linear_atten,
																			   light->quad_atten);
					}
					Lights[light_idx].atten = glm::vec4(
						light->const_atten,
						light->linear_atten,
						light->quad_atten,
						distanceCutOff
					);
					Lights[light_idx].direction = glm::quat(transform) * glm::vec4(light->direction, 1.f);
					Lights[light_idx].param1 = glm::vec4(
						light->type,
						light->inner_angle,
						light->outer_angle,
						1.f
					);
					Lights[light_idx].param2 = glm::vec4(
						light->on, 1.f, 1.f, 1.f
					);


					light_idx++;
					if (NUMBEROFLIGHTS == light_idx) break;
				}
			}
			for (; light_idx < NUMBEROFLIGHTS; light_idx++)
			{
				Lights[light_idx].param2.x = 0.f;
			}

			glBindBuffer(GL_UNIFORM_BUFFER, light_buffer);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(sLight) * NUMBEROFLIGHTS, Lights);

			return true;
		}

		void cRenderer::EntitySinglePassSort()
		{
			for (size_t idx = 0; idx < mRenderEntities.size() - 1; idx++)
			{
				if (mRenderEntities[idx]->HasComponent(Component::MULTI_TRANSFORM_COMPONENT))
				{
					std::swap(mRenderEntities[idx], mRenderEntities[idx + 1]);
				}
				else if (mRenderEntities[idx+1]->HasComponent(Component::MULTI_TRANSFORM_COMPONENT))
				{
				}
				else if (glm::distance(View->position, GetPosition(mRenderEntities[idx])) > glm::distance(View->position, GetPosition(mRenderEntities[idx + 1])))
				{
					std::swap(mRenderEntities[idx], mRenderEntities[idx + 1]);
				}
			}
		}

		glm::vec3 cRenderer::GetPosition(Entity::cEntity* entity)
		{

			if (Component::iComponent* comp = entity->GetComponent(Component::POSITION_COMPONENT))
			{
				return dynamic_cast<Component::Position*>(comp)->position;
			}
			else
			{
				glm::mat4 transform = dynamic_cast<Component::Transform*>(
					entity->GetComponent(Component::TRANSFORM_COMPONENT)
					)->transform;
				return glm::vec3(transform[3].x, transform[3].y, transform[3].z);
			}
		}

		glm::mat4 cRenderer::GetTransform(Entity::cEntity* entity)
		{
			if (Component::iComponent* comp = entity->GetComponent(Component::TRANSFORM_COMPONENT))
			{
				return dynamic_cast<Component::Transform*>(comp)->transform;
			}
			else
			{
				glm::vec3 position = dynamic_cast<Component::Position*>(
					entity->GetComponent(Component::POSITION_COMPONENT)
					)->position;

				glm::mat4 transform = glm::translate(glm::mat4(1.f), position);

				if (comp = entity->GetComponent(Component::ROTATION_COMPONENT))
				{
					transform *= glm::mat4(dynamic_cast<Component::Rotation*>(comp)->rotation);
				}

				return transform;
			}
		}


	}
}

