#pragma once

#include <glad/glad.h>

#include "../Shaders/cShaderManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm.hpp>


namespace Degen
{
	namespace Render
	{
		class cTextRenderer
		{
			struct Character
			{
				GLuint		TextureID;
				glm::ivec2	Size;
				glm::ivec2	Bearing;
				GLuint		Advance;
			};

			struct RenderString
			{
				RenderString(const std::string& text, const GLfloat& x, const GLfloat& y, const GLfloat& scale, const glm::vec3& colour)
					: text(text), x(x), y(y), scale(scale), colour(colour)
				{
				}
				float time;
				std::string text;
				GLfloat x;
				GLfloat y;
				GLfloat scale;
				glm::vec3 colour;
			};

		public:
			cTextRenderer(Shaders::cShaderManager::cShaderProgram* shader);
			void AddText(const std::string& text, const GLfloat& x, const GLfloat& y, const GLfloat& scale, const glm::vec3& colour, float time = 0.0);
			void Update(double dt);
			~cTextRenderer();
		private:
			void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 colour);

			std::vector<RenderString> mStrings;
			
			Shaders::cShaderManager::cShaderProgram* mShaderProgram = nullptr;
			FT_Library _ft;
			FT_Face _face;
			std::map<GLchar, Character> mCharacters;
			GLuint VAO, VBO;
		};
	}
}
