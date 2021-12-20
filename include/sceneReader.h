#pragma once
#include "tinyxml2.h"
#include "Transform.h"
#include "Camera.h"
#include "Skybox.h"
#include "Resource.h"
#include "GameObject.h"
#include "Light.h"
#include <stdio.h>
#include <vector>
#include <string>

struct objectNum {
	int nanosuit;
	int earth;
	int cottage;
	int cube;
	int ground;
	int lightCube;
	int container;
	int tower;
};

class sceneReader {
public:
	sceneReader(const char* path);
	void readCamera(Camera* camera);
	Skybox* readSkyBox();
	GameObject* readGameObject();
	Light::PointLight* readPointLight();
	Light::DirLight* readDirLight();
	void setTransform(Transform* transform);

	objectNum objNum;
private:
	const char* filePath;
	const char* query;
	
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement *root;
	tinyxml2::XMLElement *scene;//场景多了可以改成vector
	tinyxml2::XMLElement *cameraReader;
	tinyxml2::XMLElement *skyBoxReader;
	tinyxml2::XMLElement *gameObjectReader;
	tinyxml2::XMLElement *object;
	tinyxml2::XMLElement *pointLightReader;
	tinyxml2::XMLElement *dirLightReader;
	tinyxml2::XMLElement *element;

	void readVec3(float* x, float* y, float* z);
	
};