#pragma once
#include"graphic.h"
#include"Light.h"


class Shader {

public:
	Shader();
	~Shader();
	std::vector<glm::vec4> Viewplanes;
	Texture* texture;
private:
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectMatrix;

public:
	V2F VertexShader(const Vertex& a2v);
	//现在直接输出点的颜色
 //	glm::vec4 FragmentShader(const V2F& v);

	void setModelMatrix(const glm::mat4& model);

	void setViewMatrix(const glm::mat4& view);

	void setProjectMatrix(const glm::mat4& project);

	void UpdateViewPlanes();

	glm::vec4 FragmentShader(const V2F& v);
};