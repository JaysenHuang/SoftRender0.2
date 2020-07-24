#pragma once
#include"graphic.h"
class Material {
public:
	glm::vec4 Color;
	glm::vec4 Specular;
	int Gloss;
	Shader *shader;
	Texture* MainTex;

	Material();
	Material(const glm::vec4& color, const glm::vec4& specular, const int& gloss);
	~Material();

	void SetShader(Shader* s);

	void SetTexture(Texture* t);
};