#pragma once


#include<iostream>
#include <string>
#include "SoftRender/glm/glm/glm.hpp"
#include "SoftRender/glm/glm/gtc/matrix_transform.hpp"
#include "SoftRender/glm/glm/gtc/type_ptr.hpp"


class  Texture
{
public:
	int Width;
	int Height;
	int Channels;
	unsigned char* data;
	Texture();
	Texture(const std::string& Path);
	~Texture();
	
	void LoadTexture(const std::string& Path);

	glm::vec4 Sample2D(const glm::vec2& texcoord);

	glm::vec4 GetColor(int x, int y);


};

