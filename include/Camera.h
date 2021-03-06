#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include "Input.h"
#include "Light.h"
#include "MK14.h"
#include "P1911.h"

// FPS风格的摄像机
class Camera {
private:
    Light::SpotLight* light;
    GameObject* gun;
public:
    Transform* transform;
    float zoom = 45;
    bool camMoved;

    double xpos = 0, ypos = 0;
    double sensitivity = 0.05;
    glm::vec3 rotation, rotationWithRecoil;

    Gun* primary, *secondary , *currentGun;
    Camera();
    glm::mat4 GetViewMatrix();
    void ProcessInput(glm::vec3& translate);
    void SetTransform(const Transform& transform);
    void BindSpotLight(Light::SpotLight* light);
    void BindGun(GameObject* gun);
    void SetPosition(const glm::vec3& translate, const glm::vec3& dir);
};

#endif