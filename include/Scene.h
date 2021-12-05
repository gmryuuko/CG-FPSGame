#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <GameObject.h>
#include <Camera.h>

class Scene {
public:
	Camera* mainCamera;
	std::vector<GameObject*> gameObjects;

	void ProcessInput();
};

#endif // !SCENE_H
