#pragma once
#include"graphic.h"

class Mesh {
public:
	std::vector<Vertex> VBO;
	std::vector<unsigned int>EBO;

	Mesh();
	Mesh(const int& vNum, const int& iNum);
	~Mesh();

	Mesh(const Mesh& mesh);

	Mesh& operator= (const Mesh & mesh);

	Mesh& operator+= (const Mesh& mesh);

	void AddMesh(const Mesh& mesh);

	void AddTriangle(const Vertex &v1,const Vertex &v2,const Vertex &v3);

};
Mesh CreatePlane(const glm::vec3& leftTop, const glm::vec3& leftBottom, const glm::vec3& rightBottom, const glm::vec3& rightTop, const glm::vec3& normal);

Mesh CreateBox(const glm::vec3& center, float radius);