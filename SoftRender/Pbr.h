#pragma once
#include"Globel.h"

class Pbr {
public:
//	float calculateSpecularComponent();
	glm::vec3 result;

	Pbr(float NdotH, float NdotV, float NdotL, float LdotH, float VdotH, glm::vec3 F0, glm::vec3 Albedo, float metalness, float roughness, glm::vec3 LightColor);

	float PI = 3.14159265f;

	float D_GGX_TR(float NdotH, float a);//D

	float GeometrySchlickGGX(float NdotV, float k);//V

	float GeometrySmith(float NdotV, float NdotL, float k);

	glm::vec3 fresnelSchlick(float cosTheta,float metalness,glm::vec3 Albedo, glm::vec3 F0); //F
	
	
};