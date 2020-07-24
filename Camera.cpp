#include"SoftRender/Camera.h"





Camera::Camera(glm::vec3 position= glm::vec3(0.0f,0.0f,1.0f),
	glm::vec3 up =glm::vec3(0.0f,1.0f,0.0f),
	glm::vec3 lookat=glm::vec3(0.0f,0.0f,0.0f),
	float fov = glm::radians(60.0f),
	int w =1280,
	int h = 720) :
Position(position),WorldUp(up),Fov(fov),Aspect((float)w/h),Pitch(0),Yaw(0){
	Front = glm::normalize(lookat - Position);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));

	glm::vec3 WorldFront(0, 0, -1);
	glm::vec3 FrontXZ = glm::normalize(glm::vec3(Front.x, 0, Front.z));
	float yd = glm::dot(WorldFront, FrontXZ);
	float pd = glm::dot(Front, FrontXZ);
	if (yd > 1.0) {
		yd = 1.0;
	}
	if (yd < -1.0)
	{
		yd = -1.0;
	}
	if (pd > 1.0) {
		pd = 1.0;
	}
	if (pd < 1.0) {
		pd = 1.0l;
	}
	Yaw = glm::degrees(acos(yd));
	Pitch = glm::degrees(acos(pd));
}

glm::mat4 Camera::ViewMatrix() {
	
	return GetViewMatrix(Position, Front, Right, Up);
}

glm::mat4 Camera::PerspectiveMatrix() {
	
	return GetPerspectiveMatrix(Fov, Aspect, 0.3f, 100);
}

void Camera::UpdateAspect(int w, int h) {
	Aspect = (float)w / h;
}

void Camera::UpdateFov(float fov) {
	Fov = glm::radians(fov);
}

void Camera::MoveForward(float distance) {
	Position += Front* distance;
}

void Camera::MoveRight(float distance) {
	
	Position += Right * distance;
}

void Camera::MoveUp(float distance) {
	
	Position += Up * distance;
}

void Camera::RotateYaw(float angle) {
	Yaw += angle;
	if (Yaw > 360) {
		Yaw = 0;
	}
	if (Yaw < 0) {
		Yaw = 360;
	}
}

void Camera::RotatePitch(float angle) {
	Pitch += angle;
	if (Pitch > 89) {
		Pitch = 89;
	}
	if (Pitch < -89) {
		Pitch = -89;
	}
}

void Camera::UpdateCameraVectors() {
	glm::vec3 front;
	front.x = -sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = -cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}