#include "cRenderer.h"
#include <glad/glad.h>
#include "../Globals.h"
#include "../VAO and Meshes/sModelDrawInfo.h"

namespace Degen
{
	namespace Render
	{
		struct buffer
		{
			buffer() :
				matModel(1.f),
				matModelInverseTranspose(1.f),
				diffuseColour(1.f),
				specularColour(1.f, 1.f, 1.f, 1000.f),
				boolModifiers(GL_FALSE)
			{
			}
			glm::mat4 matModel;
			glm::mat4 matModelInverseTranspose;

			glm::vec4 diffuseColour;
			glm::vec4 specularColour;
			glm::vec4 boolModifiers;
		};


		bool cRenderer::SingleRender(Object::iGameObject* object)
		{
			glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//glUniform1i(shaderProgram->getUniformLocID("passNumber"), 1);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glDisable(GL_STENCIL_TEST);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

			glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -50.0f),
										 glm::vec3(0.0f, 0.0f, 0.0f),
										 glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(mShaderProgram->GetUniformLocationID("matView"), 1, GL_FALSE, glm::value_ptr(view));


			glUniform1f(mShaderProgram->GetUniformLocationID("Width"), WINDOW_WIDTH);
			glUniform1f(mShaderProgram->GetUniformLocationID("Height"), WINDOW_HEIGHT);



			if (object->GetType() == Object::RENDER_ONLY_TYPE)
			{
				RenderObject(dynamic_cast<Object::cRenderOnlyObject*>(object),mShaderProgram);
				return true;
			}
			return false;
		}

		void cRenderer::RenderObject(Object::cRenderOnlyObject* object, Shaders::cShaderManager::cShaderProgram* shader_program, glm::mat4 parent_matrix)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glCullFace(GL_BACK);
			
			glEnable(GL_DEPTH_TEST);		// Turn ON depth test
			glDepthMask(GL_TRUE);			// Write to depth buffer

			glm::mat4 matWorldCurrentGO = parent_matrix * (object->Transform() * glm::scale(glm::mat4(1.f), object->scale));
			glUniformMatrix4fv(shader_program->GetUniformLocationID("matModel"), 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));

			glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matWorldCurrentGO));
			glUniformMatrix4fv(shader_program->GetUniformLocationID("matModelInverseTranspose"), 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

			glUniform4f(shader_program->GetUniformLocationID("diffuseColour"),
						object->diffuse_colour.r,
						object->diffuse_colour.g,
						object->diffuse_colour.b,
						object->diffuse_colour.a);

			glUniform4f(shader_program->GetUniformLocationID("specularColour"),
						object->specular_colour.r,
						object->specular_colour.g,
						object->specular_colour.b,
						object->specular_colour.a);	// Power+


			float doNotLight = object->ignore_lighting;

			if (object->is_wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
				doNotLight = GL_TRUE;
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
			}
			float useDiffuse = true; //pCurrentObject->UseDiffuse();

			glUniform4f(shader_program->GetUniformLocationID("boolModifiers"),
						0.f,
						doNotLight,
						useDiffuse,
						0);


			glUniform1f(shader_program->GetUniformLocationID("isSkinnedMesh"), (float)GL_FALSE);


			VAOAndModel::sModelDrawInfo drawInfo;
			if (VAO->FindDrawInfoByModelName(object->mesh, drawInfo))
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
}

