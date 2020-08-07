#pragma once
#include"Shader.h"
#include"Object.h"
class Shadow {
public:

	Camera LightSpaceCam;
	Shadow(DirectionLight* L, Object& o);
	~Shadow();
	bool IsInShadow(V2F& v);
	bool IsVisibleToLight(V2F& v, DirectionLight* L);
	void Fit2Scene(Object& o);
	float CurrentDepth(float x, float y);
	std::vector<glm::vec3> GetSceneAABBCorners(Object& o);
};