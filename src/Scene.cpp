#include "Scene.h"
#include "Graphic.h"
#include "Hitbox.h"

using namespace std;
using namespace glm;

Scene::Scene() {
	mainCamera = nullptr;

    // 默认武器
    gun = new MK14();
}

void Scene::ProcessInput() {
    mat4 projection = glm::perspective(glm::radians(this->mainCamera->zoom), (float)Graphic::retWindowWidth() / (float)Graphic::retWindowHeight(), 0.1f, 100.0f);
    mat4 view = this->mainCamera->GetViewMatrix();
    mat4 invertedViewProjectionMatrix = glm::inverse(projection * view);

    glm::vec3 position, dir;
    glm::vec4 modelPos;
    
	mainCamera->ProcessInput(position);
   
    // 视角碰撞检测
    int flag = true; // translate是否合法
    if (mainCamera->camMoved) {
        modelPos = vec4(mainCamera->transform->GetPosition(), 1.0);
     
        for (auto iter : this->gameObjects) {
            if (iter->isInside(modelPos)) {
                flag = false;
                break;
            }
        }
        if (flag == false) {
            mainCamera->transform->SetPosition(modelPos);
            //cout << "Inside!\n";
        }
        mainCamera->camMoved = false;
        
    }
    //std::cout << "The mat is:" << position.x << ", " << position.y << ", " << position.z << std::endl;

    // Handling Mouse clicks
    if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {    
        vec4 nearPoint = vec4(0, 0, -1, 1);
        vec4 farPoint = vec4(0, 0, 1, 1);
        nearPoint = invertedViewProjectionMatrix * nearPoint;
        farPoint = invertedViewProjectionMatrix * farPoint;

        divideByW(nearPoint);
        divideByW(farPoint);

        vec4 viewDir = farPoint - nearPoint;
        vec4 viewPos = vec4(this->mainCamera->transform->GetPosition(), 1);
        //viewPos = vec4(vec3(viewPos) + normalize(vec3(viewDir)), 1);
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
            if (hitObject->damagable) {
                hitObject->HP -= gun->damage;
            }
        }
        else cout << "Miss!\n";
        hitObject = nullptr;
    }
}



bool Scene::canCameraMove() {
    return true;
}