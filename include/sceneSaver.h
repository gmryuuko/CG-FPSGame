#pragma once
#include <vector>
#include <map>
#include "tinyxml2.h"
#include "glm/glm.hpp"
#include "GameObject.h"
#include "sceneReader.h"
#include "Light.h"



class sceneSaver {
public:
	sceneSaver(const char* path);
	void saveCamera();
	void saveSkyBox();
	void saveGameObject(GameObject* obj);
	void saveGameObjectList(int totalNum);
	void savePointLight(Light::PointLight* pl);
	void saveDirLight(Light::DirLight* dl);
	void save();

	std::map<std::string, int>objMapNum;
private:
	const char* savePath;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* root;
	tinyxml2::XMLElement* scene;
	tinyxml2::XMLElement* camera;
	tinyxml2::XMLElement* skyBox;
	tinyxml2::XMLElement* gameObject;
	tinyxml2::XMLElement* pointLight;
	tinyxml2::XMLElement* dirLight;
	void saveTransform(tinyxml2::XMLElement* element, glm::vec3 p, glm::vec3 r, glm::vec3 s);
	void saveVec3(tinyxml2::XMLElement* element, glm::vec3 v);
};