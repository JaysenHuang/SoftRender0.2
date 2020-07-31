#pragma once
#include"Globel.h"
#include"Math.h"
#include"Material.h"
class Light {
public :
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec3 Specular;
	glm::vec3 Direction;
	float Intensity;

	virtual ~Light() {
	}
	Light(const glm::vec3& pos,
		const glm::vec3& color,
		const glm::vec3& specular,
		const glm::vec3& dir,
		const float& i) ;
	


};

class DirectionLight : public Light {
public:
	DirectionLight();
	~DirectionLight();
	DirectionLight(
		const glm::vec3& dir,
		const glm::vec3& color,
		const glm::vec3& specular,
		const float& i);

	glm::vec3 CalcDirLight(
		const glm::vec3& worldNormal,
		const glm::vec3& worldViewDir,
		const glm::vec3& albedo
	);

}; 

class PointLight : public Light {
public:
	float Constant;
	float Linear;
	float Quadratic;
	PointLight(
		const glm::vec3& pos,
		const glm::vec3& color,
		const glm::vec3& specualr,
		const float& i,
		const float& c,
		const float& l,
		const float& q
	);

	glm::vec3 CalcPointLight(
		const glm::vec3& worldPos,
		const glm::vec3& worldNormal,
		const glm::vec3& worldViewDir,
		const glm::vec3& albedo
	);
};

class SpotLight : public PointLight {
public :
	float innerCut;
	float outterCut;

	
	SpotLight(
		const glm::vec3& pos,
		const glm::vec3& dir,
		const glm::vec3& color,
		const glm::vec3& specular,
		const float& i,
		const float& c,
		const float& l,
		const float& q,
		const float& icut,
		const float& ocut);
	
	glm::vec3 CalcSpotLight(
		const glm::vec3& worldPos,
		const glm::vec3& worldNormal,
		const glm::vec3& worldViewDir,
		const glm::vec3& albedo
	);
};