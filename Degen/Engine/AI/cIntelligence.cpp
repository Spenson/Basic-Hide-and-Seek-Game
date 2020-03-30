#include "cIntelligence.h"
#include "BMP Loader/BMPImage.h"
#include "BMP Loader/ResourceManager.h"

ResourceManager gResourceManager;

namespace Degen
{
	namespace AI
	{
		cIntelligence::cIntelligence(std::string map_file)
		{
			BMPImage* bmp = new BMPImage(map_file);

			char* data = bmp->GetData();
			unsigned long imageWidth = bmp->GetImageWidth();
			unsigned long imageHeight = bmp->GetImageHeight();

			std::vector<unsigned char> test(data, data + 800);
			
			int index = 0;
			unsigned char r, g, b;
			for (unsigned long x = 0; x < imageHeight; x++)
			{
				for (unsigned long y = 0; y < imageWidth; y++)
				{
					r = data[index++];
					g = data[index++];
					b = data[index++];
					printf("(%u,%u,%u)",r,g,b);
				}
				printf("\n");
			}
		}

		void cIntelligence::Update(double dt)
		{
		}

		void cIntelligence::AddEntity(Entity::cEntity* entity)
		{
		}

	}
}

