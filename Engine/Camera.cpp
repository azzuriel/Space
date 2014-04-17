#pragma once
#include "Camera.h"
#include <gtc\matrix_transform.inl>

Camera::Camera()
{
	fov = 45.0f;
	windowWidth = 1;
	windowHeight = 1;
	aspect = float(windowWidth) / float(windowHeight);
	zNear = 0.1f;
	zFar = 100000.f;

	perspectiveProjection = glm::perspective(fov, aspect, zNear, zFar);
	view = glm::lookAt
		(
		glm::vec3(0,0,1), 
		glm::vec3(0,0,0), 
		glm::vec3(0,1,0)
		);
	rotx = 0;
	roty = 0;
	rotatex = glm::mat4(1.0f);
	position = vec3(0,0,0);
}

void Camera::SetWindowSize( unsigned int width, unsigned int height )
{
	windowWidth = width;
	windowHeight = height;
	aspect = float(windowWidth) / float(windowHeight);

	perspectiveProjection = glm::perspective(fov, aspect, zNear, zFar);
}

glm::mat4 Camera::CalculateMatrix()
{
	glm::mat4 rotatey = glm::rotate(glm::mat4(1.0f), roty, glm::vec3(1.0f, 0.0f, 0.0f));
	return perspectiveProjection * rotatey * view;
}

Camera::~Camera(void)
{

}
