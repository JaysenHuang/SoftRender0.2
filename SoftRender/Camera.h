#pragma once

#include"Math.h"


class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Size;
	float Fov;
	float Aspect;
	float Pitch;
	float Yaw;
	float N;
	float F;
	Camera();

	Camera(
		glm::vec3 position,
		glm::vec3 up,
		glm::vec3 lookat,
		float fov,
		int w,
		int h);

	glm::mat4 ViewMatrix();

	glm::mat4 PerspectiveMatrix();

	glm::mat4 PerspectiveMatrixO();

	void UpdateAspect(int w, int h);

	void UpdateFov(float fov);

	void MoveForward(float distance);
	
	void MoveUp(float distance);

	void MoveRight(float distance);

	void RotateYaw(float anlge);

	void RotatePitch(float angle);

	void UpdateCameraVectors();

private:
	

};