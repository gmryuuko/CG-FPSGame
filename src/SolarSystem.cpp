#include "SolarSystem.h"

#include <iostream>

#include "FrameTime.h"
#include "Resource.h"

using namespace std;
using namespace glm;

SolarSystem::SolarSystem() {
    rMars = vec3(3, 4, 4), sMars = 1.5;
    rEarth = vec3(7, 0.5, 8), sEarth = 2;
    rMoon = vec3(3, 0, 4), sMoon = 2;

    // set a camera
    mainCamera = new Camera();
    mainCamera->SetTransform(Transform(glm::vec3(0, 2, 7)));
    // sun
    sun = new GameObject(Resource::GetModel("sun/Sun.obj"));
    sun->isLight = true;
    sun->transform->SetScale(glm::vec3(2.0 / 20000));
    // mars
    mars = new GameObject(Resource::GetModel("mars/Mars 2K.obj"));
    mars->SetParent(sun);
    mars->transform->SetPosition(glm::vec3(rMars.x * cos(0), rMars.y * cos(0), rMars.z * sin(0)));
    mars->transform->SetScale(glm::vec3(1.0 / 5));
    // earth
    earth = new GameObject(Resource::GetModel("earth/Earth 2K.obj"));
    earth->SetParent(sun);
    earth->transform->SetPosition(glm::vec3(rEarth.x * cos(0), rEarth.y * cos(0), rEarth.z * sin(0)));
    earth->transform->SetScale(glm::vec3(1.0 / 4));
    // moon
    moon = new GameObject(Resource::GetModel("moon/Moon 2K.obj"));
    moon->SetParent(earth);
    moon->transform->SetPosition(glm::vec3(rMoon.x * cos(0), rMoon.y * cos(0), rMoon.z * sin(0)));
    moon->transform->SetScale(glm::vec3(1.0 / 8));
    // set light
    auto plight = new Light::PointLight();
    plight->position = glm::vec3(0);
    plight->ambient = glm::vec3(0.3, 0.3, 0.3);
    plight->diffuse = glm::vec3(1, 1, 1);
    plight->specular = glm::vec3(1, 1, 1);
    // skybox
    this->skybox = new Skybox(vector<string>{
        "cubemap/universe/1k_px.jpg",
        "cubemap/universe/1k_nx.jpg",
        "cubemap/universe/1k_py.jpg",
        "cubemap/universe/1k_ny.jpg",
        "cubemap/universe/1k_pz.jpg",
        "cubemap/universe/1k_nz.jpg",
    });

    pointLights.emplace_back(plight);
    gameObjects.emplace_back(sun);
}

void SolarSystem::ProcessInput() {
    if (mainCamera == nullptr) return;

    mainCamera->ProcessInput();

    static int go = true;
    static float aMars = 0, aEarth = 0, aMoon = 0;
    
    if (Input::GetKeyDown(GLFW_KEY_SPACE))
        go = !go;

    if (go) {
        float dt = FrameTime::GetDeltaTime();
        aMars += sMars * dt;
        mars->transform->SetPosition(vec3(rMars.x * cos(aMars), rMars.y * cos(aMars), rMars.z * sin(aMars)));
        aEarth += sEarth * dt;
        earth->transform->SetPosition(vec3(rEarth.x * cos(aEarth), rEarth.y * cos(aEarth), rEarth.z * sin(aEarth)));
        aMoon += sMoon * dt;
        moon->transform->SetPosition(vec3(rMoon.x * cos(aMoon), rMoon.y * cos(aMoon), rMoon.z * sin(aMoon)));
    }
}