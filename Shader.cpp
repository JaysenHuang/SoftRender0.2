#include"SoftRender/Shader.h"


//Class Shader

Shader::Shader() {
	ModelMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ProjectMatrix = glm::mat4(1.0f);
	Viewplanes.resize(6, glm::vec4(0));
	Texture* texture;
}

Shader::~Shader() = default;

V2F Shader::VertexShader(const Vertex & a2v) {
	V2F o;
	o.worldPos = ModelMatrix * a2v.position;
	// PVM*v
	o.windowPos = ProjectMatrix * ViewMatrix * o.worldPos;

	o.Z = 1 / o.windowPos.w;
	o.worldPos = o.worldPos * o.Z;
	o.color = a2v.color * o.Z;
	o.normal = a2v.normal * o.Z;
	o.texcoord = a2v.texcoord * o.Z;
	return o;
}

/*glm::vec4 Shader::FragmentShader(const V2F& v) {
	glm::vec4 color;
	color = v.color;
	return color;
}*/

void Shader::setModelMatrix(const glm::mat4 & model) {
	ModelMatrix = model;
}

void Shader::setViewMatrix(const glm::mat4 & view) {
	ViewMatrix = view;
}

void Shader::setProjectMatrix(const glm::mat4 & project) {
	ProjectMatrix = project;
}

glm::vec4 Shader::FragmentShader(const V2F & v) {
	glm::vec4 albedo = currentMat->Color;
	if (!currentMat->MainTex) {
		albedo = currentMat->Color;
	}
	else {
		albedo = currentMat->MainTex->Sample2D(v.texcoord) * currentMat->Color;
	}
	glm::vec3 worldNormal = glm::normalize(v.normal);
	glm::vec3 worldViewDir = glm::normalize(camera.Position - glm::vec3(v.worldPos));
	
	glm::vec3 result = Ambient * glm::vec3(albedo);
	if (dirLight) {
		result += dirLight->CalcDirLight(worldNormal, worldViewDir, albedo);
	}
	if (pointLight) {
		result += pointLight->CalcPointLight(v.worldPos,worldNormal,worldViewDir,albedo);
	}
	if (spotLight) {
		result += spotLight->CalcSpotLight(v.worldPos, worldNormal, worldViewDir, albedo);
	}
	result = glm::clamp(result, 0.0f, 1.0f);
	return glm::vec4(result, 1.0);
}

void Shader::UpdateViewPlanes() {
	ViewingFrustumPlanes(Viewplanes, ProjectMatrix * ViewMatrix);
}



