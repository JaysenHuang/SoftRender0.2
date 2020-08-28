#pragma once
#include "..\SoftRender\graphic.h"
#include "..\SoftRender\Mesh.h"
#include "Model.h"
#include "setting.h"
#include "Globel.h"
#include "shader.h"
void setModelMatrix(const glm::mat4& model);
void setViewMatrix(const glm::mat4& view);
void setProjectMatrix(const glm::mat4& project) ;
bool FaceCulling(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3);
bool ViewCull(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3);
void DrawLine(const V2F& from, const V2F& to);

void DrawObject(Object &obj);
void DrawModel(Model& model);
