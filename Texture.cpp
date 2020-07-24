#include"SoftRender/Texture.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "SoftRender\stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "SoftRender/stb_image.h"

Texture::Texture() = default;

Texture::Texture(const std::string & Path) {
	
	data = stbi_load(Path.c_str(), &Width, &Height, &Channels, 0);
	
}
Texture::~Texture() {
	if (data)
		return;//free(data);
}


void Texture::LoadTexture(const std::string& Path) {
	if (data)
		free(data);

	data = stbi_load("C:\\Users\\jiasheng.huang\\Documents\\GitHub\\MySoftRender\\SoftRender\\Assets\\Avatar_Loli_Catalyst_Klee_Tex_Body_Diffuse.tga", &Width, &Height, &Channels, 0);
}




glm::vec4 Texture::Sample2D(const glm::vec2& texcoord) {

	float x = texcoord.x - (float)floor(texcoord.x);
	float y = texcoord.y - (float)floor(texcoord.y);
	return GetColor(x * Width, y * Height)/255.0f;


}

glm::vec4 Texture::GetColor(int x, int y) {
	if (x <0 || x > Width || y < 0 || y> Height)
		return glm::vec4(0, 0, 0, 0);
	int xy = Channels * (y * Width + x);
		return glm::vec4(*(data + xy), *(data + xy + 1), *(data + xy + 2), *(data + xy + 3));

	
	//return glm::vec4(225, 225, 225, 0);
}