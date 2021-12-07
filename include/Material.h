#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"
#include <glm/glm.hpp>

struct Material {
    glm::vec3 colorAmbient;
    glm::vec3 colorDiffuse;
    glm::vec3 colorSpecular;
    float shininess;
    int illum;
    Texture* texDiffuse;
    Texture* texSpecular;
    Texture* texNormal;
};

const Material defaultMaterial = {
    glm::vec3(0.1, 0.1, 0.1),
    glm::vec3(0.6, 0.6, 0.6),
    glm::vec3(0.0, 0.0, 0.0),
    0,
    1,
    nullptr,
    nullptr,
    nullptr
};

#endif  // MATERIAL_H