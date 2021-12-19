#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <GameObject.h>
#include <Camera.h>
#include "Light.h"
#include "Skybox.h"

class Scene {
public:

	Camera* mainCamera = nullptr;
	Skybox* skybox = nullptr;
	std::vector<GameObject*> gameObjects;
	std::vector<Light::PointLight*> pointLights;
	std::vector<Light::DirLight*> dirLights;
	std::vector<Light::SpotLight*> spotLights;

	Scene();
	virtual void ProcessInput();
};

#endif // !SCENE_H
