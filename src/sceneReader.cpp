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
	camera->rotation = (glm::vec3(x,y,z));
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

bool sceneReader::setIsHitable() {
	tinyxml2::XMLElement* temp = object->FirstChildElement("hitable");
	if (temp) return true;
	else return false;
}

bool sceneReader::setIsDamagable() {
	tinyxml2::XMLElement* temp = object->FirstChildElement("damagable");
	if (temp) return true;
	else return false;
}

void sceneReader::setHitbox(std::vector<Hitbox*>* hitbox) {
	tinyxml2::XMLElement* hitbox_center = object->FirstChildElement("hitbox_center");
	tinyxml2::XMLElement* hitbox_size = object->FirstChildElement("hitbox_size");
	tinyxml2::XMLElement* temp = element; // 保存element指针
	float x, y, z, a, b, c;
	glm::vec3 center;
	while (hitbox_center && hitbox_size) {
		element = hitbox_center;
		readVec3(&x, &y, &z);

		element = hitbox_size;
		readVec3(&a, &b, &c);
		hitbox->push_back(new Hitbox(glm::vec3(x, y, z), a, b, c));

		// 读取下一个
		hitbox_center = hitbox_center->NextSiblingElement("hitbox_center");
		hitbox_size = hitbox_size->NextSiblingElement("hitbox_size");
	}
	// 虽然不是必须的但是还是恢复一下之前的element指针
	element = temp;

}

Scene& readFromXML(const char* path) {
	Scene& scene = *(new Scene());
	sceneReader sr(path);
	// camera
	scene.mainCamera = new Camera();
	//scene.mainCamera->transform->SetPosition(glm::vec3(0, 2, 5));
	sr.readCamera(scene.mainCamera);
	// skybox
	//sr.readSkyBox(scene.skybox);
	scene.skybox = sr.readSkyBox();
	//gameObject
	GameObject* gObj;
	for (int i = 0; i < sr.totalNum; i++) {
		gObj = sr.readGameObject();
		gObj->name = sr.tmpName;
		gObj->category = sr.tmpCategory;
		gObj->modelPath = sr.tmpModelPath;
		sr.setTransform(gObj->transform);
		gObj->isLight = sr.setIsLight();
		sr.setHitbox(&(gObj->hitboxes));
		gObj->hitable = sr.setIsHitable();
		gObj->damagable = sr.setIsDamagable();
		scene.gameObjects.emplace_back(gObj);
	}
	//dirLight
	auto dirLight = sr.readDirLight();
	scene.dirLights.emplace_back(dirLight);
	//    scene.dirLights.emplace_back(dirLight);
	//pointLight
	auto pointLight = sr.readPointLight();
	scene.pointLights.emplace_back(pointLight);


	// spot
	/*
	* auto spotLight = new Light::SpotLight{
		glm::vec3(-5, 5, -5),
		glm::vec3(1, -1, 1),
		glm::cos(glm::radians(12.5f)),
		glm::cos(glm::radians(17.5f)),
		glm::vec3(0),
		glm::vec3(0.8, 0.8, 0.8),
		glm::vec3(1, 1, 1),
		0, 0, 0
	};
	*/
	
	auto flashLight = new Light::SpotLight{
		glm::vec3(-5, 5, -5),
		glm::vec3(0.1, -1, 0.1),
		glm::cos(glm::radians(12.5f)),
		glm::cos(glm::radians(17.5f)),
		glm::vec3(0),
		glm::vec3(0.8, 0.8, 0.8),
		glm::vec3(1, 1, 1),
		1, 0.09, 0.032
	};

	//scene.spotLights.emplace_back(spotLight);
	scene.spotLights.emplace_back(flashLight);

	scene.mainCamera->BindSpotLight(flashLight);



	return scene;
}