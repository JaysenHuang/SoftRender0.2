#include"SoftRender/Pbr.h"
//D

Pbr::Pbr(float NdotH, float NdotV, float NdotL,float LdotH,float VdotH, glm::vec3 F0, glm::vec3 Albedo, float metalness,float roughness,glm::vec3 LightColor) {
	F0 = Albedo * metalness + (1 - metalness) * F0;
	roughness += 0.01;
	float Kspec = F0.x;
	float Kdiff = (1.0 - F0.x) * (1.0 - metalness);
	//diffuse
	//float fd90 = 0.5 + 2 * LdotH * LdotH * roughness* roughness;
///	float nlPow5 = glm::pow((1 - NdotL),5);
	///float nvPow5 = glm::pow((1 - NdotV),5);
	///glm::vec3 diffuse = (1 + (fd90 - 1) * nlPow5) * (1 + (fd90 - 1) * nvPow5) * LightColor;

	float K = (roughness + 1) * (roughness + 1) / 8;
	float D = D_GGX_TR(NdotH, roughness);
	float G = GeometrySmith(NdotV, NdotL, K);
	glm::vec3 F = fresnelSchlick(VdotH, metalness, Albedo, F0);
	glm::vec3  SpecularResult = (D * G * F * 0.25f) / (NdotV * NdotL);

	glm::vec3 diffuse = Albedo * Kdiff * LightColor * NdotL;
	glm::vec3 specular = SpecularResult * LightColor * NdotL * PI;
	result = (diffuse + specular);


	//result = glm::vec3(G, G, G);
	//result = Kdiff * diffuse / PI + Kspec*(D_GGX_TR(NdotH, roughness) * GeometrySmith(NdotV, NdotL, K) * fresnelSchlick(NdotH, metalness, Albedo, F0));
	//result *= NdotL;
	
}
float Pbr::D_GGX_TR(float NdotH , float a) {
	float a2 = a * a;
//	float NdotH = glm::max(glm::dot(N, H), 0.0f);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}
//V
float Pbr::GeometrySchlickGGX(float NdotV, float k) {
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float Pbr::GeometrySmith(float NdotV, float NdotL,  float k) {
	//float NdotV = glm::max(glm::dot(N, V), 0.0f);
	//float NdotL = glm::max(glm::dot(N, L), 0.0f);
	float ggx1 = NdotV / (NdotV * (1.0 - k) + k);
	float ggx2 = NdotL / (NdotL * (1.0 - k) + k);

	return ggx1 * ggx2;
}

//F
glm::vec3 Pbr::fresnelSchlick(float VdotH, float metalness, glm::vec3 Albedo, glm::vec3 F0) {
	//F0 = glm::mix(F0, Albedo, metalness);
	//float temp = glm::pow(1.0 - NdotH, 5.0);
	float temp = exp2(-5.55473 * VdotH - 6.98316 * VdotH);
	return F0 + (glm::vec3(1.0, 1.0, 1.0) - F0) * glm::vec3(temp,temp,temp);
}
