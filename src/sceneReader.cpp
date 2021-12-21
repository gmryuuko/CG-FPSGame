#include "sceneReader.h"

void sceneReader::initMap() {
	tinyxml2::XMLElement* tmp = gameObjectReader->FirstChildElement("list");
	totalNum = tmp->IntAttribute("totalNum");
	tmp = tmp->FirstChildElement();
	while (tmp) {
		objMapNum[tmp->Name()] = tmp->IntAttribute("num");
		tmp = tmp->NextSiblingElement();
	}
}

sceneReader::sceneReader(const char* path) {
	//tinyxml2::XMLError ret;
	filePath = path;
	doc.LoadFile(filePath);
	//if(ret==0) printf("load XML file failed!\n")
	root = doc.RootElement();
	scene = root->FirstChildElement();
	cameraReader = scene->FirstChildElement("camera");
	//const char* test = cameraReader->FindAttribute("name")->Name();
	//std::cout << cameraReader->FindAttribute("name")->Name() << ":" << cameraReader->FindAttribute("name")->Value() << std::endl;
	skyBoxReader = scene->FirstChildElement("skyBox");
	gameObjectReader = scene->FirstChildElement("gameObject");
	object = gameObjectReader->FirstChildElement();
	initMap();
	pointLightReader = scene->FirstChildElement("pointLight");
	dirLightReader = scene->FirstChildElement("dirLight");
}

void sceneReader::readVec3(float* x,float* y,float* z) {
	query = element->FindAttribute("x")->Value();
	sscanf(query, "%f", x);
	query = element->FindAttribute("y")->Value();
	sscanf(query, "%f", y);
	query = element->FindAttribute("z")->Value();
	sscanf(query, "%f", z);
}

void sceneReader::setTransform(Transform* transform) {
	float x, y, z;
	element = element->NextSiblingElement("position");
	readVec3(&x, &y, &z);
	transform->SetPosition(glm::vec3(x, y, z));
	element = element->NextSiblingElement("rotation");
	readVec3(&x, &y, &z);
	transform->SetRotation(glm::vec3(x, y, z));
	element = element->NextSiblingElement("scale");
	readVec3(&x, &y, &z);
	transform->SetScale(glm::vec3(x, y, z));
}

void sceneReader::readCamera(Camera* camera) {
    float x,y,z;
	//std::vector<const tinyxml2::XMLAttribute*>querys;
	element = cameraReader->FirstChildElement("position");
	readVec3(&x, &y, &z);
    camera->transform->SetPosition(glm::vec3(x,y,z));
	element = cameraReader->FirstChildElement("rotation");
	readVec3(&x, &y, &z);
	camera->transform->SetRotation(glm::vec3(x,y,z));
	element = cameraReader->FirstChildElement("scale");
	readVec3(&x, &y, &z);
	camera->transform->SetScale(glm::vec3(x,y,z));
}

Skybox* sceneReader::readSkyBox() {
	std::vector<std::string>path;
	std::string tmp;
	element = skyBoxReader->FirstChildElement("px");
	tmp = element->Attribute("path");
	path.push_back(tmp);
	element = skyBoxReader->FirstChildElement("nx");
	tmp = element->Attribute("path");
	path.push_back(tmp);
	element = skyBoxReader->FirstChildElement("py");
	tmp = element->Attribute("path");
	path.push_back(tmp);
	element = skyBoxReader->FirstChildElement("ny");
	tmp = element->Attribute("path");
	path.push_back(tmp);
	element = skyBoxReader->FirstChildElement("pz");
	tmp = element->Attribute("path");
	path.push_back(tmp);
	element = skyBoxReader->FirstChildElement("nz");
	tmp = element->Attribute("path");
	path.push_back(tmp);
	return new Skybox(path);
}

GameObject* sceneReader::readGameObject() {
	//std::string tmp;
	object = object->NextSiblingElement();
	tmpName = object->Attribute("name");
	tmpCategory = object->Name();
	element = object->FirstChildElement("model");
	tmpModelPath = element->Attribute("path");
	//object = object->NextSiblingElement();
	return new GameObject(Resource::GetModel(tmpModelPath));
}

Light::PointLight* sceneReader::readPointLight() {
	float constant;
	float linear;
	float quadratic;
	float x, y, z;
	element = pointLightReader->FirstChildElement("position");
	readVec3(&x, &y, &z);
	glm::vec3 position(x,y,z);
	element = pointLightReader->FirstChildElement("ambient");
	readVec3(&x, &y, &z);
	glm::vec3 ambient(x, y, z);
	element = pointLightReader->FirstChildElement("diffuse");
	readVec3(&x, &y, &z);
	glm::vec3 diffuse(x, y, z);
	element = pointLightReader->FirstChildElement("specular");
	readVec3(&x, &y, &z);
	glm::vec3 specular(x, y, z);
	element = pointLightReader->FirstChildElement("constant");
	constant = element->FloatAttribute("value");
	element = pointLightReader->FirstChildElement("linear");
	linear = element->FloatAttribute("value");
	element = pointLightReader->FirstChildElement("quadratic");
	quadratic = element->FloatAttribute("value");
	return new Light::PointLight{position,ambient,diffuse,specular,constant, linear, quadratic};
}

Light::DirLight* sceneReader::readDirLight() {
	float constant;
	float linear;
	float quadratic;
	float x, y, z;
	element = dirLightReader->FirstChildElement("position");
	readVec3(&x, &y, &z);
	glm::vec3 position(x, y, z);
	element = dirLightReader->FirstChildElement("direction");
	readVec3(&x, &y, &z);
	glm::vec3 direction(x, y, z);
	element = dirLightReader->FirstChildElement("ambient");
	readVec3(&x, &y, &z);
	glm::vec3 ambient(x, y, z);
	element = dirLightReader->FirstChildElement("diffuse");
	readVec3(&x, &y, &z);
	glm::vec3 diffuse(x, y, z);
	element = dirLightReader->FirstChildElement("specular");
	readVec3(&x, &y, &z);
	glm::vec3 specular(x, y, z);
	return new Light::DirLight{ position,direction,ambient,diffuse,specular};
}

bool sceneReader::setIsLight() {
	element = element->NextSiblingElement("light");
	bool ret=element->BoolAttribute("isLight");
	return ret;
}