#include "sceneSaver.h"

sceneSaver::sceneSaver(const char* path) {
	savePath = path;
	tinyxml2::XMLDeclaration* declaration = doc.NewDeclaration();
	doc.InsertFirstChild(declaration);
	root = doc.NewElement("root");
	doc.InsertEndChild(root);
	scene = doc.NewElement("scene");
	root->InsertEndChild(scene);
	camera= doc.NewElement("camera");
	scene->InsertEndChild(camera);
	skyBox = doc.NewElement("skyBox");
	scene->InsertEndChild(skyBox);
	gameObject = doc.NewElement("gameObject");
	scene->InsertEndChild(gameObject);
	pointLight = doc.NewElement("pointLight");
	scene->InsertEndChild(pointLight);
	dirLight = doc.NewElement("dirLight");
	scene->InsertEndChild(dirLight);
}

void sceneSaver::save() {
	doc.SaveFile(savePath);
}

void sceneSaver::saveVec3(tinyxml2::XMLElement* element, glm::vec3 v) {
	element->SetAttribute("x", v.x);
	element->SetAttribute("y", v.y);
	element->SetAttribute("z", v.z);
}

void sceneSaver::saveTransform(tinyxml2::XMLElement* element,glm::vec3 p,glm::vec3 r,glm::vec3 s) {
	tinyxml2::XMLElement* position=doc.NewElement("position");
	tinyxml2::XMLElement* rotation = doc.NewElement("rotation");
	tinyxml2::XMLElement* scale = doc.NewElement("scale");
	saveVec3(position, p);
	saveVec3(rotation, r);
	saveVec3(scale, s);
	element->InsertEndChild(position);
	element->InsertEndChild(rotation);
	element->InsertEndChild(scale);
}

void sceneSaver::saveCamera() {
	saveTransform(camera,glm::vec3(0.0,2.0,5.0),glm::vec3(0.0,0.0,0.0), glm::vec3(0.0, 0.0, 0.0));
}

void sceneSaver::saveSkyBox() {
	tinyxml2::XMLElement* px = doc.NewElement("px");
	tinyxml2::XMLElement* nx = doc.NewElement("nx");
	tinyxml2::XMLElement* py = doc.NewElement("py");
	tinyxml2::XMLElement* ny = doc.NewElement("ny");
	tinyxml2::XMLElement* pz = doc.NewElement("pz");
	tinyxml2::XMLElement* nz = doc.NewElement("nz");
	px->SetAttribute("path", "cubemap/universe/1k_px.jpg");
	nx->SetAttribute("path", "cubemap/universe/1k_nx.jpg");
	py->SetAttribute("path", "cubemap/universe/1k_py.jpg");
	ny->SetAttribute("path", "cubemap/universe/1k_ny.jpg");
	pz->SetAttribute("path", "cubemap/universe/1k_pz.jpg");
	nz->SetAttribute("path", "cubemap/universe/1k_nz.jpg");
	skyBox->InsertEndChild(px);
	skyBox->InsertEndChild(nx);
	skyBox->InsertEndChild(py);
	skyBox->InsertEndChild(ny);
	skyBox->InsertEndChild(pz);
	skyBox->InsertEndChild(nz);
}

void sceneSaver::saveGameObject(GameObject* obj) {
	tinyxml2::XMLElement* object = doc.NewElement(obj->category.c_str());
	object->SetAttribute("name", obj->name.c_str());
	tinyxml2::XMLElement* model = doc.NewElement("model");
	model->SetAttribute("path", obj->modelPath.c_str());
	object->InsertEndChild(model);
	saveTransform(object, obj->transform->GetPosition(), obj->transform->GetRotation(), obj->transform->GetScale());
	tinyxml2::XMLElement* hitbox_center = doc.NewElement("hitbox_center");
	tinyxml2::XMLElement* hitbox_size = doc.NewElement("hitbox_size");
	for (auto iter : obj->hitboxes) {
		saveVec3(hitbox_center, iter->center);
		saveVec3(hitbox_size, glm::vec3(iter->x, iter->y, iter->z));
		object->InsertEndChild(hitbox_center);
		object->InsertEndChild(hitbox_size);

		// 构建一组新的hitbox条目，因为可能有多个hitbox
		hitbox_center = doc.NewElement("hitbox_center");
		hitbox_size = doc.NewElement("hitbox_size");
	}
	tinyxml2::XMLElement* light = doc.NewElement("light");
	light->SetAttribute("isLight", obj->isLight);
	object->InsertEndChild(light);
	if (obj->hitable) {
		tinyxml2::XMLElement* hitable = doc.NewElement("hitable");
		object->InsertEndChild(hitable);
	}
	if (obj->hitable && obj->damagable) {
		tinyxml2::XMLElement* damagable = doc.NewElement("damagable");
		object->InsertEndChild(damagable);
	}
	gameObject->InsertEndChild(object);
	//tinyxml2::XMLElement* num = doc.NewElement("num");
}

void sceneSaver::saveGameObjectList(int totalNum) {
	tinyxml2::XMLElement* list = doc.NewElement("list");
	list->SetAttribute("totalNum", totalNum);
	tinyxml2::XMLElement* tmpElement;
	const char* ct;
	for (auto iter = objMapNum.begin(); iter != objMapNum.end(); iter++) {
		ct = iter->first.c_str();
		tmpElement = doc.NewElement(ct);
		tmpElement->SetAttribute("num", objMapNum[ct]);
		list->InsertEndChild(tmpElement);
	}
	gameObject->InsertFirstChild(list);
}

void sceneSaver::savePointLight(Light::PointLight* pl) {
	tinyxml2::XMLElement* position = doc.NewElement("position");
	tinyxml2::XMLElement* ambient = doc.NewElement("ambient");
	tinyxml2::XMLElement* diffuse = doc.NewElement("diffuse");
	tinyxml2::XMLElement* specular = doc.NewElement("specular");
	tinyxml2::XMLElement* constant = doc.NewElement("constant");
	tinyxml2::XMLElement* linear = doc.NewElement("linear");
	tinyxml2::XMLElement* quadratic = doc.NewElement("quadratic");
	saveVec3(position, pl->position);
	saveVec3(ambient, pl->ambient);
	saveVec3(diffuse, pl->diffuse);
	saveVec3(specular, pl->specular);
	constant->SetAttribute("value", pl->constant);
	linear->SetAttribute("value", pl->linear);
	quadratic->SetAttribute("value", pl->quadratic);
	pointLight->InsertEndChild(position);
	pointLight->InsertEndChild(ambient);
	pointLight->InsertEndChild(diffuse);
	pointLight->InsertEndChild(specular);
	pointLight->InsertEndChild(constant);
	pointLight->InsertEndChild(linear);
	pointLight->InsertEndChild(quadratic);
}

void sceneSaver::saveDirLight(Light::DirLight* dl) {
	tinyxml2::XMLElement* position = doc.NewElement("position");
	tinyxml2::XMLElement* direction = doc.NewElement("direction");
	tinyxml2::XMLElement* ambient = doc.NewElement("ambient");
	tinyxml2::XMLElement* diffuse = doc.NewElement("diffuse");
	tinyxml2::XMLElement* specular = doc.NewElement("specular");
	saveVec3(position, dl->position);
	saveVec3(direction, dl->direction);
	saveVec3(ambient, dl->ambient);
	saveVec3(diffuse, dl->diffuse);
	saveVec3(specular, dl->specular);
	dirLight->InsertEndChild(position);
	dirLight->InsertEndChild(direction);
	dirLight->InsertEndChild(ambient);
	dirLight->InsertEndChild(diffuse);
	dirLight->InsertEndChild(specular);
}