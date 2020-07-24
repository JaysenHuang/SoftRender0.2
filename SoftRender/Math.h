#pragma once
#include"glm\glm\glm.hpp"
#include"glm\glm\gtc\matrix_transform.hpp"
#include<vector>
#include<iostream>

glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, float factor);
glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, float factor);
glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, float factor);
float Lerp(const float& f1, const float& f2, float factor);
glm::mat4 GetViewPortMatrix(int ox, int oy, int width, int height);
glm::mat4 GetViewMatrix(glm::vec3 pos,glm::vec3 front,glm::vec3 right,glm::vec3 up);
glm::mat4 GetPerspectiveMatrix(const float& fovy, const float& aspect, const float n, const float& f);
void ViewingFrustumPlanes(std::vector<glm::vec4> &result, const glm::mat4 &vp);
bool Point2Plane(glm::vec4& p, glm::vec3& v);
