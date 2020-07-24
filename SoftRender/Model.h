#pragma once
#include"Object.h"

class Model {
public:
	std::vector<Object> objects;
	Model();
	~Model();

	Model(const std::string& path);

	Model(const Model& model);

	Model& operator=(const Model& model);

	void SetMaterial(const int& id, const Material& m);

	void LoadObj(const std::string& path);
};