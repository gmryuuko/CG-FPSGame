#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "Shader.h"

namespace Light {
    // 点光源
    struct PointLight {
        // 位置
        glm::vec3 position;
        // 环境光颜色
        glm::vec3 ambient;
        // 漫反射光颜色
        glm::vec3 diffuse;
        // 镜面光颜色
        glm::vec3 specular;
        //
        float constant;
        float linear;
        float quadraic;
    };

    void SetPointLight(Shader& shader, int index, PointLight* light);
}

#endif  // LIGHT_H