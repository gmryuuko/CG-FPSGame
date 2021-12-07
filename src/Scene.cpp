#include "Scene.h"

using namespace std;

Scene::Scene() {
	mainCamera = nullptr;
}

void Scene::ProcessInput() {
	mainCamera->ProcessInput();

}
