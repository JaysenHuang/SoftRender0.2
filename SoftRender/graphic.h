#pragma once
#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_


#include"Globel.h"
#include"Math.h"
#include"Texture.h"
#include"Camera.h"

//Starts up SDL and creates window
extern bool init();

//Frees media and shuts down SDL
extern void close();


//The window we'll be rendering to

void SDLDrawPixel(int x, int y);


class FrameBuffer {

public:
	int Width, Height;

	std::vector<float> colorBuffer;
	std::vector<float> depthBuffer;
	
	
	~FrameBuffer();

	FrameBuffer(const int& w, const int& h);

	void Resize(const int& w, const int& h);

	void ClearColorBuffer(const glm::vec4& color);

	void ClearDepthBuffer();

	void WritePoint(const int x, const int y, const glm::vec4 color, SDL_Renderer* gRenderer);

	float GetDepth(const int& x, const int& y);

	void WriteDepth(const int& x, const int& y, const float& depth);
};

class Vertex {
public:
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec3 normal;

	Vertex();
	~Vertex();

	Vertex(
		const glm::vec4& _pos,
		const glm::vec4& _color,
		const glm::vec2& _tex,
		const glm::vec3& _normal
	);

	Vertex(
		const glm::vec3& _pos,
		const glm::vec4& _color,
		const glm::vec2& _tex,
		const glm::vec3& _normal
	);

	Vertex(const Vertex& v);
};

class V2F {
public:
	glm::vec4 worldPos;
	glm::vec4 windowPos;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
	float Z;	
	V2F();

	~V2F();

	V2F(
		const glm::vec4& _wPos,
		const glm::vec4& _pPos,
		const glm::vec4& _color,
		const glm::vec2& _tex,
		const glm::vec3& _normal
		
	);

	V2F(const V2F& v);

	
	static V2F lerp(const V2F& v1, const V2F& v2, const float& factor);

	
};

class Shader {

public:
	Shader();
	~Shader();
	std::vector<glm::vec4> Viewplanes;
	Texture* texture;
private:
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectMatrix;
	
public:
	V2F VertexShader(const Vertex& a2v);
	//现在直接输出点的颜色
 //	glm::vec4 FragmentShader(const V2F& v);

	void setModelMatrix(const glm::mat4& model);

	void setViewMatrix(const glm::mat4& view);

	void setProjectMatrix(const glm::mat4& project);

	void UpdateViewPlanes();

	glm::vec4 FragmentShader(const V2F& v,Texture texture);
};




void ScanLine(const V2F& left, const V2F& right);

void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3,int i);

void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3, int i);

void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3, int i);

void PerspectiveDivision(V2F& v);
//void DrawMesh(const Mesh& mesh);

#endif // 