#include "Camera.h"
#include "FrameTime.h"

#include "glm/gtx/string_cast.hpp"

using namespace glm;

Camera::Camera() {
    this->transform = new Transform();
    light = nullptr;
    camMoved = false;
}

mat4 Camera::GetViewMatrix() {
    // 用Transform每次还要计算model矩阵，好像开销有点大
    vec3 position = transform->GetPosition();
    return lookAt(position, position - transform->GetAxisZ(), vec3(0, 1, 0));
    // return lookAt(vec3(0, 0, 6), vec3(0, 0, 0), vec3(0, 1, 0));
}

void Camera::ProcessInput(glm::vec3 &position) {
    
    // keyboard
    static float moveSpeed = 5.0f;

    // speed control
    if (Input::GetKeyDown(GLFW_KEY_UP)) {
        moveSpeed = moveSpeed >= 10 ? moveSpeed : moveSpeed + 1;
    }
    if (Input::GetKeyDown(GLFW_KEY_DOWN)) {
        moveSpeed = moveSpeed <= 1 ? moveSpeed : moveSpeed - 1;
    }

    // move control
    vec3 right = transform->GetAxisX();
    vec3 front = -transform->GetAxisZ();
    vec3 up = vec3(0, 1, 0);
    right.y = 0;
    front.y = 0;
    right = normalize(right);
    front = normalize(front);
    vec3 direction = vec3(0, 0, 0);

    if (Input::GetKey(GLFW_KEY_W)) {
        camMoved = true;
        direction += front;
    }
    if (Input::GetKey(GLFW_KEY_S)) {
        camMoved = true;
        direction -= front;
    }
    if (Input::GetKey(GLFW_KEY_A)) {
        camMoved = true;
        direction -= right;
    }
    if (Input::GetKey(GLFW_KEY_D)) {
        camMoved = true;
        direction += right;
    }
    if (Input::GetKey(GLFW_KEY_R)) {
        camMoved = true;
        direction += up;
    }
    if (Input::GetKey(GLFW_KEY_F)) {
        camMoved = true;
        direction -= up;
    }

    // std::cout << glm::to_string(direction) << std::endl;
    // 在实现位移之前，我们先存一下之前的translate的position，这样在scene里面如果判定碰撞则可以返回
    
    if (camMoved && direction != vec3(0, 0, 0)) {
        position = transform->GetPosition();
        
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


    // 移动绑定的聚光灯
    if (light != nullptr) {
        light->position = transform->GetPosition();
        light->direction = -transform->GetAxisZ();
    }
}

void Camera::SetTransform(const Transform& transform) {
    *(this->transform) = transform;
}

void Camera::BindSpotLight(Light::SpotLight* light) {
    this->light = light;
}
void Camera::SetPosition(const glm::vec3& position, const glm::vec3& dir) {
    this->transform->SetPosition(position);
    //this->transform->Translate(dir, -0.01);
    if (light != nullptr) {
        light->position = position;
    }
}
