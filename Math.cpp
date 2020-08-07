#include "SoftRender/Math.h"

glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, float factor) {
	return (1.0f - factor) * v1 + factor * v2;
}
glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, float factor) {
	return (1.0f - factor) * v1 + factor * v2;
}
glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, float factor) {
	return (1.0f - factor) * v1 + factor * v2;
}
float Lerp(const float& f1, const float& f2, float factor) {
	return(1.0f - factor) * f1 + factor * f2;
}
glm::mat4 GetViewPortMatrix(int ox, int oy, int width, int height) {
	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = width / 2.0f;
	result[3][0] = ox + (width / 2.0f);
	result[1][1] = height / 2.0f;
	result[3][1] = oy + (height / 2.0f);
	return result;
}

glm::mat4 GetViewMatrix(glm::vec3 pos, glm::vec3 front, glm::vec3 right, glm::vec3 up) {
	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = right.x;
	result[1][0] = right.y;
	result[2][0] = right.z;
	result[3][0] = -glm::dot(right, pos);
	result[0][1] = up.x;
	result[1][1] = up.y;
	result[2][1] = up.z;
	result[3][1] = -glm::dot(up, pos);
	result[0][2] = -front.x;
	result[1][2] = -front.y;
	result[2][2] = -front.z;
	result[3][2] = glm::dot(front, pos);
	return result;
}

glm::mat4 GetPerspectiveMatrix(const float& fovy, const float& aspect, const float n, const float& f) {
	glm::mat4 result = glm::mat4(0.0f);
	const float tanHalfFov = tan(fovy * 0.5f);
	result[0][0] = 1.0f / (aspect * tanHalfFov);
	result[1][1] = 1.0f / (tanHalfFov);
	result[2][2] = -(f + n) / (f - n);
	result[2][3] = -1.0f;
	result[3][2] = (-2.0f * n * f) / (f - n);
	return result;
}
glm::mat4 GetPerspectiveMatrixO(const float& size, const float& aspect, const float& n, const float& f) {
	glm::mat4 result = glm::mat4(0.0f);
	result[0][0] = 1.0f / (size * aspect);
	result[1][1] = 1.0f / size;
	result[2][2] = -2.0f / (f - n);
	result[3][2] = -(f + n) / (f - n);
	result[3][3] = 1.0f;
	return result;
} 

void ViewingFrustumPlanes(std::vector<glm::vec4>& result, const glm::mat4& vp) {
		//×ó²à
		result[0].x = vp[0][3] + vp[0][0];
		result[0].y = vp[1][3] + vp[1][0];
		result[0].z = vp[2][3] + vp[2][0];
		result[0].w = vp[3][3] + vp[3][0];
		//ÓÒ²à
		result[1].x = vp[0][3] - vp[0][0];
		result[1].y = vp[1][3] - vp[1][0];
		result[1].z = vp[2][3] - vp[2][0];
		result[1].w = vp[3][3] - vp[3][0];
		//ÉÏ²à
		result[2].x = vp[0][3] - vp[0][1];
		result[2].y = vp[1][3] - vp[1][1];
		result[2].z = vp[2][3] - vp[2][1];
		result[2].w = vp[3][3] - vp[3][1];
		//ÏÂ²à
		result[3].x = vp[0][3] + vp[0][1];
		result[3].y = vp[1][3] + vp[1][1];
		result[3].z = vp[2][3] + vp[2][1];
		result[3].w = vp[3][3] + vp[3][1];
		//Near
		result[4].x = vp[0][3] + vp[0][2];
		result[4].y = vp[1][3] + vp[1][2];
		result[4].z = vp[2][3] + vp[2][2];
		result[4].w = vp[3][3] + vp[3][2];
		//Far
		result[5].x = vp[0][3] - vp[0][2];
		result[5].y = vp[1][3] - vp[1][2];
		result[5].z = vp[2][3] - vp[2][2];
		result[5].w = vp[3][3] - vp[3][2];
	}
bool Point2Plane(glm::vec4& p, glm::vec3& v) {
	return p.x * v.x + p.y * v.y + p.z * v.z + p.w >= 0;
}
