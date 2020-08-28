#include "SoftRender/Light.h"


Light::Light(const glm::vec3& pos = glm::vec3(0, 0, 0),
		const glm::vec3& color = glm::vec3(1, 1, 1),
		const glm::vec3& specular = glm::vec3(1, 1, 1),
		const glm::vec3& dir = glm::vec3(0, -1, 0),
		const float& i = 1.0f):Position(pos),
		Color(color),
		Specular(specular),
		Direction(dir),
		Intensity(i){}

DirectionLight::DirectionLight(const glm::vec3& dir = glm::normalize(glm::vec3(1, 1, 1)),
	const glm::vec3& color = glm::vec3(1, 1, 1),
	const glm::vec3& specular = glm::vec3(1, 1, 1),
	const float& i = 0.5f) {
	Position = glm::vec3(-5, 5, 2.5);
	Color = color;
	Specular = specular;
	Direction = dir;
	Intensity = i;
}
DirectionLight::DirectionLight() = default;
DirectionLight::~DirectionLight() = default;
 glm::vec3 DirectionLight::CalcDirLight(
	const glm::vec3& worldNormal,
	const glm::vec3& worldViewDir,
	const glm::vec3& albedo,
	const glm::vec3& smbe
)
{
	/* float diff = glm::max(glm::dot(worldNormal, -Direction), 0.0f);
	glm::vec3 reflectDir = glm::normalize(reflect(Direction, worldNormal));
	float spec =pow(glm::max(glm::dot(worldViewDir, reflectDir), 0.0f), currentMat->Gloss);
	
	glm::vec3 diffuse = Color * diff * albedo;
	glm::vec3 specular = Specular * spec;
	glm::vec3 result = (diffuse + specular) * Intensity;*/
	glm::vec3 lightDir = glm::normalize(Position);
	glm::vec3 halfDirection = glm::normalize(worldViewDir + lightDir);
    float NdotL = glm::clamp(glm::dot(worldNormal, lightDir),0.0f,1.0f);
	float LdotH = glm::clamp(glm::dot(lightDir, halfDirection), 0.0f, 1.0f);
	float NdotV = abs(dot(worldNormal, worldViewDir));
	float NdotH = glm::clamp(glm::dot(worldNormal, halfDirection), 0.0f, 1.0f);
	float VdotH = glm::clamp(glm::dot(worldViewDir, halfDirection), 0.0f, 1.0f);
	float roughness = 1.0 - smbe.r;
	float metalness = smbe.g;
	pbr = new Pbr(NdotH, NdotV, NdotL, LdotH,VdotH, glm::vec3(0.04f, 0.04f, 0.04f), albedo, metalness, roughness, Color * Intensity);
	
	return pbr->result;
}

 PointLight::PointLight(
	 const glm::vec3& pos =glm::vec3(2,2,2),
	 const glm::vec3& color =glm::vec3(1,1,1),
	 const glm::vec3& specualr =glm::vec3(1,1,1),
	 const float& i =1.0f,
	 const float& c =1.0f,
	 const float& l =0.09f,
	 const float& q =0.032f
 ) :Constant(c),Linear(l),Quadratic(q){
	 Position = pos;
	 Color = color;
	 Specular = specualr;
	 Direction = glm::vec3(0, 0, 0);
	 Intensity = i;
 }

 glm::vec3 PointLight::CalcPointLight(
	 const glm::vec3& worldPos,
	 const glm::vec3& worldNormal,
	 const glm::vec3& worldViewDir,
	 const glm::vec3& albedo
 ) {
	 float distance = glm::distance(worldPos, Position);
	 float atten = 1.0/ (Constant + Linear * distance + Quadratic * (distance * distance));
	 glm::vec3 lightDir = glm::normalize(worldPos - Position);
	 float diff = glm::max(glm::dot(worldNormal, -lightDir), 0.0f);
	 glm::vec3 reflectDir = glm::normalize(reflect(lightDir, worldNormal));
	 float spec = pow(glm::max(glm::dot(worldViewDir, reflectDir), 0.0f), currentMat->Gloss);

	 glm::vec3 diffuse = Color * diff * albedo;
	 glm::vec3 specular = Specular * spec;

	 diffuse *= atten;
	 specular *= atten;
	
	 glm::vec3 result = (diffuse + specular) * Intensity;
	
	 return result;
 }

 SpotLight::SpotLight(const glm::vec3& pos = glm::vec3(0,2,0),
	 const glm::vec3& dir = glm::vec3(0, -1, 0),
	 const glm::vec3& color = glm::vec3(1, 1, 1),
	 const glm::vec3& specular = glm::vec3(1,1,1) ,
	 const float& i =1.0f,
	 const float& c =1.0f,
	 const float& l = 0.09f,
	 const float& q = 0.032,
	 const float& icut = 12.5f,
	 const float& ocut = 17.5f){
	 Direction = dir;
	 Position = pos;
	 Color = color;
	 Specular = specular;
	 Intensity = i;
	 Constant = c;
	 Linear = l;
	 Quadratic = q;
	 innerCut = glm::cos(glm::radians(icut));
	 outterCut = glm::cos(glm::radians(ocut));
 }

 glm::vec3 SpotLight::CalcSpotLight(const glm::vec3& worldPos,
	 const glm::vec3& worldNormal,
	 const glm::vec3& worldViewDir,
	 const glm::vec3& albedo) {
	 float distance = glm::distance(worldPos, Position);
	 float atten = 1.0 / (Constant + Linear * distance + Quadratic * (distance * distance));

	 glm::vec3 lightDir = glm::normalize(worldPos - Position);
	 float theta = glm::dot(lightDir, glm::normalize(Direction));
	 float weight =glm::clamp((theta - outterCut) / (innerCut - outterCut),0.0f,1.0f);
	 float intensity = Lerp(0, 1, weight);
	
	 float diff = glm::max(glm::dot(worldNormal, -lightDir), 0.0f);
	 glm::vec3 reflectDir = glm::normalize(reflect(lightDir, worldNormal));
	 float spec = pow(glm::max(glm::dot(worldViewDir, reflectDir), 0.0f), currentMat->Gloss);

	 glm::vec3 diffuse = Color * diff * albedo;
	 glm::vec3 specular = Specular * spec;

	 diffuse *= (atten * intensity);
	 specular *= (atten * intensity);
	 
	 return (diffuse + Specular) * Intensity;
 }