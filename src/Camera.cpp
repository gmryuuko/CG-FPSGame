#include "Camera.h"
#include "FrameTime.h"

#include "glm/gtx/string_cast.hpp"

using namespace glm;

Camera::Camera() {
    this->transform = new Transform();
}

mat4 Camera::GetViewMatrix() {
    // 用Transform每次还要计算model矩阵，好像开销有点大
    vec3 position = transform->GetPosition();
    return lookAt(position, position - transform->GetAxisZ(), vec3(0, 1, 0));
    // return lookAt(vec3(0, 0, 6), vec3(0, 0, 0), vec3(0, 1, 0));
}

void Camera::ProcessInput() {
    // keyboard
    static float moveSpeed = 2.0f;

    vec3 right = transform->GetAxisX();
    vec3 front = -transform->GetAxisZ();
    vec3 up = vec3(0, 1, 0);
    right.y = 0;
    front.y = 0;
    right = normalize(right);
    front = normalize(front);
    vec3 direction = vec3(0, 0, 0);

    if (Input::GetKey(GLFW_KEY_W)) {
        direction += front;
    }
    if (Input::GetKey(GLFW_KEY_S)) {
        direction -= front;
    }
    if (Input::GetKey(GLFW_KEY_A)) {
        direction -= right;
    }
    if (Input::GetKey(GLFW_KEY_D)) {
        direction += right;
    }
    if (Input::GetKey(GLFW_KEY_R)) {
        direction += up;
    }
    if (Input::GetKey(GLFW_KEY_F)) {
        direction -= up;
    }

    // std::cout << glm::to_string(direction) << std::endl;

    if (direction != vec3(0, 0, 0)) {
        transform->Translate(direction, moveSpeed * FrameTime::GetDeltaTime());
    }

    // mouse
    static double xpos = 0, ypos = 0, lastXpos = 0, lastYpos = 0;
    static double sensitivity = 0.05;
    static bool first = true;
    double deltaXpos, deltaYpos;

    lastXpos = xpos;
    lastYpos = ypos;
    Input::GetCursor(xpos, ypos);

    if (first) {
        deltaXpos = 0;
        deltaYpos = 0;
        first = false;
    }
    else {
        deltaXpos = xpos - lastXpos;
        deltaYpos = ypos - lastYpos;
    }
    
    deltaXpos = deltaXpos * sensitivity;
    deltaYpos = deltaYpos * sensitivity;
    
    // 鼠标左右移动是xpos，改变yaw角
    // 鼠标上下移动是ypos，改变pitch角
    transform->RotateY(-deltaXpos);
    transform->RotateX(-deltaYpos, true);

}

void Camera::SetTransform(const Transform& transform) {
    *(this->transform) = transform;
}