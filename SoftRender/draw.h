#pragma once
#include "..\SoftRender\graphic.h"
#include "..\SoftRender\Mesh.h"
#include "SoftRender\setting.h"
#include"SoftRender/Model.h"

void setModelMatrix(const glm::mat4& model);
void setViewMatrix(const glm::mat4& view);
void setProjectMatrix(const glm::mat4& project) ;
bool FaceCulling(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3);
bool ViewCull(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3);


void DrawObject(Object &obj);
void DrawModel(Model& model);

void Draw(SDL_Renderer* gRenderer, Shader shader, FrameBuffer FrontBuffer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
void Draw2(SDL_Renderer* gRenderer, Shader shader, FrameBuffer FrontBuffer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
void MyDraw(V2F &v,SDL_Renderer* gRenderer, Shader& shader, FrameBuffer& FrontBuffer, Texture texture);