#ifndef MY_CAMERA
#define MY_CAMERA

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class My_Camera {

    float p_angle, p_width, p_height, p_near, p_far;



    float speed, biggest_rotate_speed;
    float sensitivity;

    void updateCameraFrontRight();
public:

    glm::vec3 camera_pos, camera_direction, camera_up, camera_front, camera_right;
    My_Camera();

    glm::mat4 getLookAt();
    glm::mat4 getProjection();

    glm::vec3 getPosition();

    void setSpeed(float v);
    void setWidthAndHeight(float w,float h);
    void setSensitivity(float s);
    void setPos(float x, float y, float z);

    

    void moveForward();
    void moveBack();
    void moveRight();
    void moveLeft();

    void rotate(float left, float top);
};

#endif
