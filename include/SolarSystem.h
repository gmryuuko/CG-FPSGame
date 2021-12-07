#pragma once

#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include <glm/glm.hpp>

#include "Scene.h"

class SolarSystem : public Scene {
private:
    GameObject* sun = nullptr;
    GameObject* mars = nullptr;
    GameObject* earth = nullptr;
    GameObject* moon = nullptr;
    float sMars, sEarth, sMoon;
    glm::vec3 rMars, rEarth, rMoon;

public:
    SolarSystem();
    virtual void ProcessInput();
};

#endif // SOLARSYSTEM_H