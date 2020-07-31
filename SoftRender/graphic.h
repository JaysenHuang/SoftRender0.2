#pragma once
#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include"Globel.h"
#include"Math.h"
#include"Texture.h"
#include"Camera.h"
#include<mutex>


void SwapBuffer();
//Starts up SDL and creates window
extern bool init();

//Frees media and shuts down SDL
extern void close();


//The window we'll be rendering to

void SDLDrawPixel(int x, int y);


class FrameBuffer {

public:
	int Width, Height;

	std::vector<char> colorBuffer;
	std::vector<float> depthBuffer;
	
	
	~FrameBuffer();

	FrameBuffer(const int& w, const int& h);

	void Resize(const int& w, const int& h);

	void ClearColorBuffer(const glm::vec4& color);

	void ClearDepthBuffer();

	void WritePoint(const int x, const int y, const glm::vec4 color);

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






void ScanLine(const V2F& left, const V2F& right);

void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3);

void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3);

void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3);

void PerspectiveDivision(V2F& v);
//void DrawMesh(const Mesh& mesh);

#endif // 