#include "Camera.h"
#include <glew.h>
#include <gtc\matrix_transform.inl>
#include <gtx\quaternion.hpp>

Camera::Camera() {
	camera_mode = FREE;
	up = glm::vec3(0.0, 1.0, 0.0);
	field_of_view = 45;
	rotation_quaternion = glm::quat(1.0, 0.0, 0.0, 0.0);
	position_delta = glm::vec3(0.0, 0.0, 0.0);
	camera_scale = .5f;
	max_pitch_rate = 5;
	max_heading_rate = 5;
	move_camera = false;
	near_clip = 0.1f;
	far_clip = 10000;
	auto windowWidth = 1;
	auto windowHeight = 1;
	aspect = float(windowWidth) / float(windowHeight);
}
Camera::~Camera() {
}

void Camera::Reset() {
	up = glm::vec3(0.0, 1.0, 0.0);
}

void Camera::Update() {
	direction = glm::normalize(look_at - position);
	//need to set the matrix state. this is only important because lighting doesn't work if this isn't done
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glViewport(viewport_x, viewport_y, window_width, window_height);

	if (camera_mode == ORTHO) {
		//our projection matrix will be an orthogonal one in this case
		//if the values are not floating point, this command does not work properly
		//need to multiply by aspect!!! (otherise will not scale properly)
		projection = glm::ortho(-1.5f * float(aspect), 1.5f * float(aspect), -1.5f, 1.5f, -10.0f, 10.f);
	} else if (camera_mode == FREE) {
		projection = glm::perspective(field_of_view, aspect, near_clip, far_clip);
		//detmine axis for pitch rotation
		glm::vec3 axis = glm::cross(direction, up);
		//compute quaternion for pitch based on the camera pitch angle
		glm::quat pitch_quat = glm::angleAxis(camera_pitch, axis);
		//determine heading quaternion from the camera up vector and the heading angle
		glm::quat heading_quat = glm::angleAxis(camera_heading, up);
		//add the two quaternions
		glm::quat temp = glm::cross(pitch_quat, heading_quat);
		temp = glm::normalize(temp);
		//update the direction from the quaternion
		direction = glm::rotate(temp, direction);
		//add the camera delta
		position += position_delta;
		//set the look at to be infront of the camera
		look_at = position + direction * 1.0f;
		//damping for smooth camera
		camera_heading *= .5;
		camera_pitch *= .5;
		position_delta = position_delta * .8f;
	}
	//compute the MVP
	view = glm::lookAt(position, look_at, up);
	model = glm::mat4(1.0f);
	MVP = projection * view * model;
	//glLoadMatrixf(glm::value_ptr(MVP));
}

//Setting Functions
void Camera::SetMode(CameraType cam_mode) {
	camera_mode = cam_mode;
	up = glm::vec3(0.0, 1.0, 0.0);
	rotation_quaternion = glm::quat(1.0, 0.0, 0.0, 0.0);
}

void Camera::SetPosition(glm::vec3 pos) {
	position = pos;
}

void Camera::SetLookAt(glm::vec3 pos) {
	look_at = pos;
}
void Camera::SetFOV(double fov) {
	field_of_view = fov;
}
void Camera::SetViewport(int loc_x, int loc_y, int width, int height) {
	viewport_x = loc_x;
	viewport_y = loc_y;
	window_width = width;
	window_height = height;
	//need to use doubles division here, it will not work otherwise and it is possible to get a zero aspect ratio with integer rounding
	aspect = double(width) / double(height);
	;
}
void Camera::SetClipping(double near_clip_distance, double far_clip_distance) {
	near_clip = near_clip_distance;
	far_clip = far_clip_distance;
}

void Camera::Move(CameraDirection dir) {
	if (camera_mode == FREE) {
		switch (dir) {
		case UP:
			position_delta += up * camera_scale;
			break;
		case DOWN:
			position_delta -= up * camera_scale;
			break;
		case LEFT:
			position_delta -= glm::cross(direction, up) * camera_scale;
			break;
		case RIGHT:
			position_delta += glm::cross(direction, up) * camera_scale;
			break;
		case FORWARD:
			position_delta += direction * camera_scale;
			break;
		case BACK:
			position_delta -= direction * camera_scale;
			break;
		}
	}
}
void Camera::ChangePitch(float degrees) {
	//Check bounds with the max pitch rate so that we aren't moving too fast
	if (degrees < -max_pitch_rate) {
		degrees = -max_pitch_rate;
	} else if (degrees > max_pitch_rate) {
		degrees = max_pitch_rate;
	}
	camera_pitch += degrees;

	//Check bounds for the camera pitch
	if (camera_pitch > 360.0f) {
		camera_pitch -= 360.0f;
	} else if (camera_pitch < -360.0f) {
		camera_pitch += 360.0f;
	}
}
void Camera::ChangeHeading(float degrees) {
	//Check bounds with the max heading rate so that we aren't moving too fast
	if (degrees < -max_heading_rate) {
		degrees = -max_heading_rate;
	} else if (degrees > max_heading_rate) {
		degrees = max_heading_rate;
	}
	//This controls how the heading is changed if the camera is pointed straight up or down
	//The heading delta direction changes
	if (camera_pitch > 90 && camera_pitch < 270 || (camera_pitch < -90 && camera_pitch > -270)) {
		camera_heading -= degrees;
	} else {
		camera_heading += degrees;
	}
	//Check bounds for the camera heading
	if (camera_heading > 360.0f) {
		camera_heading -= 360.0f;
	} else if (camera_heading < -360.0f) {
		camera_heading += 360.0f;
	}
}
void Camera::Move2D(int x, int y) {
	//compute the mouse delta from the previous mouse position
	glm::vec3 mouse_delta = glm::vec3(-x, -y, 0) * 3.0F;
	//if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
	if (move_camera) {
		ChangeHeading(.08f * mouse_delta.x);
		ChangePitch(.08f * mouse_delta.y);
	}
	//mouse_position = glm::vec3(x, y, 0);
}

void Camera::SetPos(int button, int state, int x, int y) {
	//if (button == 3 && state == GLUT_DOWN) {
	//	position_delta += up * .05f;
	//} else if (button == 4 && state == GLUT_DOWN) {
	//	position_delta -= up * .05f;
	//} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	//	move_camera = true;
	//} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
	//	move_camera = false;
	//}
	//mouse_position = glm::vec3(x, y, 0);
}

CameraType Camera::GetMode() {
	return camera_mode;
}

void Camera::GetViewport(int &loc_x, int &loc_y, int &width, int &height) {
	loc_x = viewport_x;
	loc_y = viewport_y;
	width = window_width;
	height = window_height;
}

void Camera::GetMatricies(glm::mat4 &P, glm::mat4 &V, glm::mat4 &M) {
	P = projection;
	V = view;
	M = model;
}

mat4 Camera::VP()
{
	return MVP;
}

glm::mat4 Camera::GetOrthoProjection()
{
	auto orthoProjection = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f, 0.1f, 100.0f);
	return orthoProjection;
}

void Camera::SetWindowSize(int l_width, int l_height)
{
	window_height = l_height;
	window_width = l_width;
	aspect = double(l_width) / double(l_height);
}
