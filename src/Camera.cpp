#include "Camera.h"

using namespace glm;

Camera::Camera(Transform* transform) {
    this->transform = transform;
}

mat4 Camera::GetViewMatrix() {
    vec3 position = transform->GetPosition();
    return lookAt(position, position - transform->GetAxisZ(), vec3(0, 1, 0));
}

