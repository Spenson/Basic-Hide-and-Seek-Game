#include "cSharedDepthFBO.h"
namespace Degen
{
	namespace Render
	{
		GLuint cSharedDepthFBO::depthTexture_ID = 0;

		// Calls shutdown(), then init()
		bool cSharedDepthFBO::reset(int width, int height, std::string& error, bool reset_depth)
		{
			if (!this->shutdown(reset_depth))
			{
				error = "Could not shutdown";
				return false;
			}

			return this->init(width, height, error, reset_depth);
		}

		bool cSharedDepthFBO::shutdown(bool shutdown_depth)
		{
			glDeleteTextures(1, &(this->texture_colour_ID));
			glDeleteTextures(1, &(this->texture_normal_ID));
			glDeleteTextures(1, &(this->texture_position_ID));
			glDeleteTextures(1, &(this->texture_specular_ID));

			if (shutdown_depth)
			{
				glDeleteTextures(1, &(this->depthTexture_ID));
			}
			
			glDeleteFramebuffers(1, &(this->ID));

			return true;
		}


		bool cSharedDepthFBO::init(int width, int height, std::string& error, bool init_depth)
		{
			this->width = width;
			this->height = height;

			//	glCreateFramebuffers(1, &( this->ID ) );	// GL 4.5		//g_FBO
			glGenFramebuffers(1, &(this->ID));		// GL 3.0
			glBindFramebuffer(GL_FRAMEBUFFER, this->ID);

			//************************************************************
				// Create the colour buffer (texture)
			glGenTextures(1, &(this->texture_colour_ID));		//g_FBO_colourTexture
			glBindTexture(GL_TEXTURE_2D, this->texture_colour_ID);


			//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8,		// 8 bits per colour
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,		// 8 bits per colour
						   this->width,				// g_FBO_SizeInPixes
						   this->height);			// g_FBO_SizeInPixes

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			// Create the normal
			glGenTextures(1, &(this->texture_normal_ID));
			glBindTexture(GL_TEXTURE_2D, this->texture_normal_ID);

			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,		// 8 bits per colour
				//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,		// 8 bits per colour
						   this->width,				// g_FBO_SizeInPixes
						   this->height);			// g_FBO_SizeInPixes

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			// Create the world pos
			glGenTextures(1, &(this->texture_position_ID));		//g_FBO_colourTexture
			glBindTexture(GL_TEXTURE_2D, this->texture_position_ID);

			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,		// 8 bits per colour
						   this->width,				// g_FBO_SizeInPixes
						   this->height);			// g_FBO_SizeInPixes

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			// Create the specular
			glGenTextures(1, &(this->texture_specular_ID));		//g_FBO_colourTexture
			glBindTexture(GL_TEXTURE_2D, this->texture_specular_ID);

			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,
						   this->width,				// g_FBO_SizeInPixes
						   this->height);			// g_FBO_SizeInPixes

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



			//***************************************************************




				// Create the depth buffer (texture1)
			if (init_depth)
			{
				glGenTextures(1, &(depthTexture_ID));			//g_FBO_depthTexture

				glBindTexture(GL_TEXTURE_2D, depthTexture_ID);


				//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, ]

				// Note that, unless you specifically ask for it, the stencil buffer
				// is NOT present... i.e. GL_DEPTH_COMPONENT32F DOESN'T have stencil

				// These are:
				// - GL_DEPTH32F_STENCIL8, which is 32 bit float depth + 8 bit stencil
				// - GL_DEPTH24_STENCIL8,  which is 24 bit float depth + 8 bit stencil (more common?)
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8,	//GL_DEPTH32F_STENCIL8,
							   this->width,		//g_FBO_SizeInPixes
							   this->height);
				//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT );
				//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_COMPONENT );
				//	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->width, this->height, 0, GL_EXT_packe

				//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_TEXTURE_2D, this->depthTexture_ID, 0);

			}
			// ***************************************************************





			glFramebufferTexture(GL_FRAMEBUFFER,
								 GL_COLOR_ATTACHMENT0,			// Colour goes to #0
								 this->texture_colour_ID, 0);

			glFramebufferTexture(GL_FRAMEBUFFER,
								 GL_COLOR_ATTACHMENT1,
								 this->texture_normal_ID, 0);

			glFramebufferTexture(GL_FRAMEBUFFER,
								 GL_COLOR_ATTACHMENT2,
								 this->texture_position_ID, 0);

			glFramebufferTexture(GL_FRAMEBUFFER,
								 GL_COLOR_ATTACHMENT3,
								 this->texture_specular_ID, 0);


			glFramebufferTexture(GL_FRAMEBUFFER,
								 GL_DEPTH_STENCIL_ATTACHMENT,
								 depthTexture_ID, 0);



			static const GLenum draw_buffers[] =
			{
				GL_COLOR_ATTACHMENT0,
				GL_COLOR_ATTACHMENT1,
				GL_COLOR_ATTACHMENT2,
				GL_COLOR_ATTACHMENT3,
			};
			glDrawBuffers(4, draw_buffers);		// There are 4 outputs now

			// ***************************************************************




			// ADD ONE MORE THING...
			bool bFrameBufferIsGoodToGo = true;

			switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
			{
				case GL_FRAMEBUFFER_COMPLETE:
					bFrameBufferIsGoodToGo = true;
					break;

				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					error = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
					bFrameBufferIsGoodToGo = false;
					break;
					//	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				case GL_FRAMEBUFFER_UNSUPPORTED:
				default:
					bFrameBufferIsGoodToGo = false;
					break;
			}//switch ( glCheckFramebufferStatus(GL_FRAMEBUFFER) )

			// Point back to default frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return bFrameBufferIsGoodToGo;
		}

		void cSharedDepthFBO::clearColourBuffer(int bufferindex)
		{
			glViewport(0, 0, this->width, this->height);
			GLfloat	zero = 0.0f;
			glClearBufferfv(GL_COLOR, bufferindex, &zero);

			return;
		}

		void cSharedDepthFBO::clearAllBuffers(void)
		{
			glViewport(0, 0, this->width, this->height);
			GLfloat	zero = 1.0f;
			GLfloat one = 1.0f;
			glClearBufferfv(GL_COLOR, 0, &zero);
			glClearBufferfv(GL_COLOR, 1, &zero);
			glClearBufferfv(GL_COLOR, 2, &zero);
			glClearBufferfv(GL_COLOR, 3, &zero);

			glClearBufferfv(GL_DEPTH, 0, &one);

			glStencilMask(0xFF);

			{	// Clear stencil
				//GLint intZero = 0;
				//glClearBufferiv(GL_STENCIL, 0, &intZero );
				glClearBufferfi(GL_DEPTH_STENCIL,
								0,		// Must be zero
								1.0f,	// Clear value for depth
								0);	// Clear value for stencil
			}
		}


		void cSharedDepthFBO::clearBuffers(bool bClearColour, bool bClearDepth)
		{
			glViewport(0, 0, this->width, this->height);
			GLfloat	zero = 0.0f;
			GLfloat one = 1.0f;
			if (bClearColour)
			{
				glClearBufferfv(GL_COLOR, 0, &zero);		// Colour
				glClearBufferfv(GL_COLOR, 1, &zero);		// Colour
				glClearBufferfv(GL_COLOR, 2, &zero);		// Colour
				glClearBufferfv(GL_COLOR, 3, &zero);		// Colour
			}
			if (bClearDepth)
			{
				glClearBufferfv(GL_DEPTH, 0, &one);		// Depth is normalized 0.0 to 1.0f

				glStencilMask(0xFF);

				{	// Clear stencil
					//GLint intZero = 0;
					//glClearBufferiv(GL_STENCIL, 0, &intZero );
					glClearBufferfi(GL_DEPTH_STENCIL,
									0,		// Must be zero
									1.0f,	// Clear value for depth
									0);	// Clear value for stencil
				}
			}
			// If buffer is GL_STENCIL, drawbuffer must be zero, and value points to a 
			//  single value to clear the stencil buffer to. Masking is performed in the 
			//  same fashion as for glClearStencil. Only the *iv forms of these commands 
			//  should be used to clear stencil buffers; be used to clear stencil buffers; 
			//  other forms do not accept a buffer of GL_STENCIL.

			//

			return;
		}


		int cSharedDepthFBO::getMaxColourAttachments(void)
		{
			//  void glGetIntegerv(GLenum pname,
			// 				       GLint * data);

			int maxColourAttach = 0;
			glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColourAttach);

			return maxColourAttach;
		}

		int cSharedDepthFBO::getMaxDrawBuffers(void)
		{
			int maxDrawBuffers = 0;
			glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);

			return maxDrawBuffers;
		}

	}
}