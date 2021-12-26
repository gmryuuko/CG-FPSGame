#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <GameObject.h>
#include "Camera.h"
#include "Light.h"
#include "Skybox.h"
#include "LittleGun.h"
#include "ParticleSystem.h"

class Scene {
public:
	ParticleSystem* flame = nullptr;
	Transform* flameTransfrom;
	Camera* mainCamera = nullptr;
	Skybox* skybox = nullptr;
	std::vector<GameObject*> gameObjects;
	std::vector<Light::PointLight*> pointLights;
	std::vector<Light::DirLight*> dirLights;
	std::vector<Light::SpotLight*> spotLights;
	Scene();
	virtual void ProcessInput();
	int GetPlayerHP() const;
private:
	bool canCameraMove();
	void enemyActions();
	void breathRecover();
	int playerHP;
	float playerLastHit;
	LittleGun* littleGun;
};

#endif // !SCENE_H
