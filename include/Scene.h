#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <GameObject.h>
#include "Camera.h"
#include "Light.h"
#include "Skybox.h"
#include "MK14.h"

class Scene {
public:
	Camera* mainCamera = nullptr;
	Skybox* skybox = nullptr;
	std::vector<GameObject*> gameObjects;
	std::vector<Light::PointLight*> pointLights;
	std::vector<Light::DirLight*> dirLights;
	std::vector<Light::SpotLight*> spotLights;
	Gun* gun;
	Scene();
	virtual void ProcessInput();
private:
	bool canCameraMove();
};

#endif // !SCENE_H
