#include "Camera.h"
#include <glew.h>
#include <gtc\matrix_transform.inl>
#include <gtx\quaternion.hpp>
#include <string>
#include "JHelpers_inl.h"
#include <gtc\quaternion.hpp>

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
    far_clip = 1000;
    auto windowWidth = 1;
    auto windowHeight = 1;
    aspect = float(windowWidth) / float(windowHeight);
camera_heading = camera_pitch = 0;
}
Camera::~Camera() {
}

void Camera::Reset() {
    up = glm::vec3(0.0, 1.0, 0.0);
}

void Camera::Update() {
    auto yawq = glm::angleAxis(camera_pitch, vec3(1,0,0));
    auto pitchq = glm::angleAxis(camera_heading, vec3(0,1,0));

    projection = glm::perspective(field_of_view, aspect, near_clip, far_clip);

    rotation_quaternion = pitchq * yawq * rotation_quaternion;
    rotation_quaternion = normalize(rotation_quaternion);

    position += position_delta;
    look_at = position + direction;


    camera_heading *= .5;
    camera_pitch *= .5;
    position_delta = position_delta * .8f;
    
    view = translate(mat4_cast(rotation_quaternion), -position);//glm::lookAt(position, look_at, up);
    up = vec3(view[1][0],view[1][1],view[1][2]);
    direction = vec3(view[0][0],view[0][1],view[0][2]);
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
    rotation_quaternion = quat_cast(lookAt(position, pos, vec3(0,1,0)));
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
           position_delta += vec3(0, -camera_scale, 0) * rotation_quaternion;
            break;
        case DOWN:
            position_delta += vec3(0, camera_scale, 0) * rotation_quaternion;
            break;
        case LEFT:
           position_delta += vec3(-camera_scale, 0, 0) * rotation_quaternion;
            break;
        case RIGHT:
           position_delta += vec3(camera_scale, 0, 0) * rotation_quaternion;
            break;
        case FORWARD:
            position_delta += vec3(0, 0, -camera_scale) * rotation_quaternion;
            break;
        case BACK:
           position_delta += vec3(0, 0, camera_scale) * rotation_quaternion;
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
    glm::vec3 mouse_delta = glm::vec3(x, y, 0) * 3.0F;
    //if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
    if (move_camera) {
        ChangeHeading(.08f * mouse_delta.x);
        ChangePitch(.08f * mouse_delta.y);
    }
    //mouse_position = glm::vec3(x, y, 0);
}

void Camera::SetPos(int button, int state, int x, int y) {
    //if (button == 3 && state == GLUT_DOWN) {
    // position_delta += up * .05f;
    //} else if (button == 4 && state == GLUT_DOWN) {
    // position_delta -= up * .05f;
    //} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // move_camera = true;
    //} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    // move_camera = false;
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

void Camera::GetMatricies(glm::mat4 &P, glm::mat4 &V, glm::mat4 &M) const {
    P = projection;
    V = view;
    M = model;
}

std::string Camera::getFullDebugDescription()
{
    return string_format("CAMERA\n%s\nheading = %s pitch = %s\nup = %s\ndir = %s\ncross = %s", 
        std::to_string(view).c_str(), 
        std::to_string(camera_heading).c_str(), 
        std::to_string(camera_pitch).c_str(),
        std::to_string(up).c_str(),
        std::to_string(direction).c_str(),
        std::to_string(glm::cross(direction, up)).c_str());
}

mat4 Camera::VP() const
{
    return projection*view;
}

glm::mat4 Camera::GetOrthoProjection()
{
    auto orthoProjection = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f, 0.1f, 100.0f);
    return orthoProjection;
}

const mat4 OrthoProjection(float left, float right,
                           float bottom, float top, float zNear, float zFar)
{
    const float tx = - (right + left) / (right - left),
        ty = - (top + bottom) / (top - bottom),
        tz = - (zFar + zNear) / (zFar - zNear);

    return mat4(2 / (right - left), 0, 0, tx,
        0, 2 / (top - bottom), 0, ty,
        0, 0, -2 / (zFar - zNear), tz,
        0, 0, 0, 1);
}

void Camera::SetWindowSize(int l_width, int l_height)
{
    window_height = l_height;
    window_width = l_width;
    aspect = double(l_width) / double(l_height);
}