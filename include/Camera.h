#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"

// FPS风格的摄像机
class Camera {

private:
    Transform* transform;

public:
    Camera(Transform* transform);
    glm::mat4 GetViewMatrix();
    void ProcessInput();
};

#endif