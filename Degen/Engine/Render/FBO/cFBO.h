#pragma once
#include <string>
#include <glad/glad.h>


namespace Degen
{
	namespace Render
	{
		class cFBO
		{
		public:
			cFBO() :
				ID(0),
				colourTexture_ID(0),
				normalTexture_ID(0),
				positionTexture_ID(0),
				specularTexture_ID(0),
				depthTexture_ID(0),
				width(-1), height(-1)
			{
			};

			GLuint ID;						// = 0;

			GLuint colourTexture_ID;		// = 0;
			GLuint normalTexture_ID;
			GLuint positionTexture_ID;
			GLuint specularTexture_ID;

			GLuint depthTexture_ID;		// = 0;
			GLint width;		// = 512 the WIDTH of the framebuffer, in pixels;
			GLint height;

			// Inits the FBP
			bool init(int width, int height, std::string& error);
			bool shutdown(void);
			// Calls shutdown(), then init()
			bool reset(int width, int height, std::string& error);

			void clearBuffers(bool bClearColour = true, bool bClearDepth = true);

			void clearColourBuffer(int bufferindex);

			void clearAllBuffers(void);

			//void clearAllColourBuffers(void);
			//void clearDepthBuffer(void);
			//void clearStencilBuffer(int clearColour, int mask = 0xFF);

			int getMaxColourAttachments(void);
			int getMaxDrawBuffers(void);
		};
	}
}
