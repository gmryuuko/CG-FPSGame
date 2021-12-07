#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include "Input.h"

// FPS风格的摄像机
class Camera {
public:
    Transform* transform;
    float zoom = 45;
    Camera();
    glm::mat4 GetViewMatrix();
    void ProcessInput();
    void SetTransform(const Transform& transform);
};

#endif