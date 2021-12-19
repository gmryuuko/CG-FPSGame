#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include "Input.h"
#include "Light.h"

// FPS风格的摄像机
class Camera {
private:
    Light::SpotLight* light;

public:
    Transform* transform;
    float zoom = 45;
    Camera();
    glm::mat4 GetViewMatrix();
    void ProcessInput();
    void SetTransform(const Transform& transform);
    void BindSpotLight(Light::SpotLight* light);
};

#endif