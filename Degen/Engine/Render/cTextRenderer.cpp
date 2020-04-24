#include "cTextRenderer.h"
#include "../Globals.h"


namespace Degen
{
	namespace Render
	{
		cTextRenderer::cTextRenderer(Shaders::cShaderManager::cShaderProgram* shader)
		{
			mShaderProgram = shader;

			if (FT_Init_FreeType(&_ft))
			{
				fprintf(stderr, "Error: could not init FreeType.\n");
			}

			if (FT_New_Face(_ft, "assets/fonts/arial.ttf", 0, &_face))
			{
				fprintf(stderr, "Error: Failed to load font assets/fonts/arial.ttf.\n");
			}

			FT_Set_Pixel_Sizes(_face, 25, 36);

			if (FT_Load_Char(_face, 'X', FT_LOAD_RENDER))
			{
				fprintf(stderr, "unable to load character\n");
			}



			glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
			// Load first 128 characters of ASCII set
			for (GLubyte c = 0; c < 128; c++)
			{
				// Load character glyph
				if (FT_Load_Char(_face, c, FT_LOAD_RENDER))
				{
					fprintf(stderr, "ERROR::FREETYTPE: Failed to load Glyph\n");
					continue;
				}

				// Generate texture
				GLuint texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);

				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					_face->glyph->bitmap.width,
					_face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					_face->glyph->bitmap.buffer
				);

				// Set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				// Now store character for later use
				Character character = {
					texture,
					glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
					glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
					_face->glyph->advance.x
				};

				mCharacters.insert(std::pair<GLchar, Character>(c, character));
			}


			FT_Done_Face(_face);
			FT_Done_FreeType(_ft);


			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

		}

		void cTextRenderer::AddText(const std::string& text, const GLfloat& x, const GLfloat& y, const GLfloat& scale, const glm::vec3& colour, float time)
		{
			mStrings.emplace_back(RenderString(text, x, y, scale, colour, time));
		}

		void cTextRenderer::Update(double dt)
		{
			for (size_t c = 0; c < mStrings.size(); c++)
			{

				RenderText(mStrings[c].text, mStrings[c].x, mStrings[c].y, mStrings[c].scale, mStrings[c].colour);

				if (mStrings[c].time == 0)
				{
					mStrings.erase(mStrings.begin() + c);
					c--;
				}
				else if (mStrings[c].time > 0.f)
				{
					mStrings[c].time -= dt;
					if (mStrings[c].time < 0)mStrings[c].time = 0;
				}
			}
			//mStrings.clear();
		}

		void cTextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 colour)
		{
			// Activate corresponding render state
			glUseProgram(mShaderProgram->ID);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

			glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WINDOW_WIDTH), 0.0f, static_cast<GLfloat>(WINDOW_WIDTH));

			glUniformMatrix4fv(glGetUniformLocation(mShaderProgram->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniform3f(glGetUniformLocation(mShaderProgram->ID, "textColor"), colour.x, colour.y, colour.z);
			glUniform1i(glGetUniformLocation(mShaderProgram->ID, "text"), 0);

			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(VAO);
			// Iterate through all characters
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character ch = mCharacters[*c];

				GLfloat xpos = x + ch.Bearing.x * scale;
				GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

				GLfloat w = ch.Size.x * scale;
				GLfloat h = ch.Size.y * scale;

				// Update VBO for each character
				GLfloat vertices[6][4] =
				{
					{ xpos, ypos + h, 0.0, 0.0 },
					{ xpos, ypos, 0.0, 1.0 },
					{ xpos + w, ypos, 1.0, 1.0 },

					{ xpos, ypos + h, 0.0, 0.0 },
					{ xpos + w, ypos, 1.0, 1.0 },
					{ xpos + w, ypos + h, 1.0, 0.0 }
				};

				// Render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);

				// Update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				// Render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);

				// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				x += (ch.Advance >> 6)* scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		cTextRenderer::~cTextRenderer()
		{

		}
	}
}