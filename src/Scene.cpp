#include "Scene.h"
#include "Graphic.h"
#include "Hitbox.h"

using namespace std;
using namespace glm;

Scene::Scene() {
	mainCamera = nullptr;
}

void Scene::ProcessInput() {
	mainCamera->ProcessInput();

    if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        // ��⵽����
        mat4 projection = glm::perspective(glm::radians(this->mainCamera->zoom), (float)Graphic::retWindowWidth()/ (float)Graphic::retWindowHeight(), 0.1f, 100.0f);
        mat4 view = this->mainCamera->GetViewMatrix();
        mat4 invertedViewProjectionMatrix = glm::inverse(projection * view);
        vec4 nearPoint = vec4(0, 0, -1, 1);
        vec4 farPoint = vec4(0, 0, 1, 1);
        nearPoint = invertedViewProjectionMatrix * nearPoint;
        farPoint = invertedViewProjectionMatrix * farPoint;

        divideByW(nearPoint);
        divideByW(farPoint);

        vec4 viewDir = farPoint - nearPoint;
        vec4 viewPos = vec4(this->mainCamera->transform->GetPosition(), 1);

        float hitDistance = INFINITY; 
        GameObject* hitObject = nullptr;
        
        for (auto iter : this->gameObjects) {
            float temp = iter->isHit(viewDir, viewPos);
            if (temp < hitDistance) {
                hitDistance = temp;
                hitObject = iter;
            }
        }

        if (hitObject != nullptr)
        {   
            if (!hitObject->name.empty())
                cout << "The object " << hitObject->name << " has been hit, at the distance of " << hitDistance << endl;
            else cout << "Hit an anoynomous object "<< "at the distance of " << hitDistance << endl;
        }
        else cout << "Miss!\n";
        hitObject = nullptr;
    }
}
