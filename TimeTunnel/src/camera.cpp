#include "camera.h"
using namespace std;



My_Camera::My_Camera() {

    camera_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    camera_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

    speed = 0.005f;
    p_width = 800;
    p_height = 600;
    p_near = 0.01f;
    p_far = 100.0f;
    p_angle = 45.0f;
    sensitivity = 3200.0f;
}

void My_Camera::updateCameraFrontRight() {

    camera_right = glm::cross(camera_direction - camera_pos, camera_up);
    camera_front = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera_right);
}

glm::mat4 My_Camera::getLookAt() {
    return glm::lookAt(camera_pos, camera_direction, camera_up);
}

glm::mat4 My_Camera::getProjection() {
    return glm::perspective(glm::radians(p_angle * 100), p_width / p_height, p_near, p_far);
}

glm::vec3 My_Camera::getPosition() {
    return camera_pos;
}

void My_Camera::setSpeed(float v) {
    speed = v;
}

void My_Camera::setWidthAndHeight(float w, float h) {
    p_width = w;
    p_height = h;
}

void My_Camera::setSensitivity(float s) {
    sensitivity = s;
}

void My_Camera::setPos(float x, float y, float z) {
    camera_direction -= camera_pos;
    camera_pos = glm::vec3(x, y, z);
    camera_direction += camera_pos;
}

void My_Camera::moveForward() {
    updateCameraFrontRight();
    camera_direction += speed * camera_front;
    camera_pos += speed * camera_front;
}

void My_Camera::moveBack() {
    updateCameraFrontRight();
    camera_direction -= speed * camera_front;
    camera_pos -= speed * camera_front;
}

void My_Camera::moveRight() {
    updateCameraFrontRight();
    camera_direction += speed * camera_right;
    camera_pos += speed * camera_right;
}

void My_Camera::moveLeft() {
    updateCameraFrontRight();
    camera_direction -= speed * camera_right;
    camera_pos -= speed * camera_right;
}

void My_Camera::rotate(float left, float top) {
    updateCameraFrontRight();
    if (left != 0) {
        glm::quat left_quat = glm::quat(left / p_width * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
        camera_direction = glm::normalize((camera_direction - camera_pos) * left_quat) + camera_pos;
        camera_up = glm::normalize(camera_up * left_quat);

    }
    updateCameraFrontRight();
    if (top != 0) {
        glm::vec3 new_camera_direction;
        glm::quat top_quat = glm::quat(top / p_height * sensitivity, camera_right);
        camera_direction = glm::normalize((camera_direction - camera_pos) * top_quat) + camera_pos;
        camera_up = glm::normalize(camera_up * top_quat);
        if (camera_up.y < 0) camera_up = -camera_up;
    }
    /*
    if ( top != 0 ) {

        glm::vec3 new_camera_direction;
        glm::quat top_quat = glm::quat(top / p_height  * sensitivity, camera_right);
        new_camera_direction = glm::normalize((camera_direction - camera_pos) * top_quat) + camera_pos;
        if ( new_camera_direction.x * new_camera_direction.x + new_camera_direction.z * new_camera_direction.z > 0.2 ) {
            camera_direction = new_camera_direction;
        }
        else {
            camera_direction.y -= (camera_direction.y - 0.0f)/8;
        }
    }
    */
}

