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
#include <map>
#include <string>

class sceneReader {
public:
	std::string tmpModelPath;
	std::string tmpName;
	std::string tmpCategory;
	int totalNum;

	sceneReader(const char* path);
	void readCamera(Camera* camera);
	Skybox* readSkyBox();
	GameObject* readGameObject();
	Light::PointLight* readPointLight();
	Light::DirLight* readDirLight();
	void setTransform(Transform* transform);
	bool setIsLight();
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
	std::map<std::string, int> objMapNum;

	void readVec3(float* x, float* y, float* z);
	void initMap();
};