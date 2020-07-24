#pragma once
#include"Mesh.h"
#include"Material.h"

class Object {
public:
	Mesh mesh;

	Material material;

	Object();
	~Object();
	Object(const Object& obj);
	Object(const Mesh& m, const Material& mat);
	Object& operator=(const Object &obj);
};