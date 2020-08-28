#include"SoftRender/Material.h"


Material::Material() :Color(glm::vec4(1.0, 1.0, 1.0, 1.0)),
Specular(glm::vec4(1.0, 1.0, 1.0, 1.0)),
Gloss(32),
shader(nullptr),
MainTex(nullptr),
SMBETex(nullptr),
NormalTex(nullptr)
 {};

Material::Material(const glm::vec4& color, const glm::vec4& specular, const int& gloss):
	Color(color),
	Specular(specular),
	Gloss(gloss),
	shader(nullptr),
	MainTex(nullptr),
	SMBETex(nullptr),
	NormalTex(nullptr)
{};
	

Material::~Material() = default;

void Material::SetShader(Shader* s) {
	shader = s;
}

void Material::SetTexture(Texture* maintex,Texture* smbe, Texture* normal) {
	MainTex = maintex;
	SMBETex = smbe;
	NormalTex = normal;
}

glm::vec3 Material::SampleNormal(const glm::vec2& texcoord) {
	if (NormalTex) {
		return NormalTex->Sample2D(texcoord);
	}
	else {
		return glm::vec3(0, 0, 1);
	}
}