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
        float quadratic;
    };

    struct DirLight {
        // 位置
        glm::vec3 position;
        // 方向
        glm::vec3 direction;
        // 环境光
        glm::vec3 ambient;
        // 漫反射
        glm::vec3 diffuse;
        // 镜面
        glm::vec3 specular;
    };

    struct SpotLight {
        // 位置
        glm::vec3 position;
        // 方向
        glm::vec3 direction;
        // 切光角
        float cutOff;
        float outerCutOff;
        // 环境光颜色
        glm::vec3 ambient;
        // 漫反射光颜色
        glm::vec3 diffuse;
        // 镜面光颜色
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };

    void SetPointLight(Shader& shader, int index, PointLight* light);
    void SetDirLight(Shader& shader, int index, DirLight* light);
    void SetSpotLight(Shader& shader, int index, SpotLight* light);
}

#endif  // LIGHT_H