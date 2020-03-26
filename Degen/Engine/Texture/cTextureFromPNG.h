#pragma once
#include <string>
#include <glad/glad.h>


class cTextureFromPNG
{
	//why not all public
public:
	cTextureFromPNG();
	~cTextureFromPNG();

	bool CreateTextureFromPNG(std::string textureName, std::string fileNameFullPath, bool bGenerateMIPMap);

	unsigned height;
	unsigned width;
	GLint m_textureNumber;
	
};
