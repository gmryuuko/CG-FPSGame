#include "Scene.h"
#include "Graphic.h"
#include "Hitbox.h"

using namespace std;
using namespace glm;

Scene::Scene() {
	mainCamera = nullptr;

    playerHP = 100;
    playerLastHit = FrameTime::GetTime();
    littleGun = new LittleGun();
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
        mainCamera->currentGun->fires();
        mainCamera->rotation = mainCamera->rotationWithRecoil;
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
            if (hitObject->damagable) {
                hitObject->HP -= mainCamera->currentGun->getDamage(hitDistance);
            }
            if (!hitObject->name.empty())
                cout << "The object " << hitObject->name << " has been hit, at the distance of " << hitDistance << ", its HP is " << hitObject->HP << endl;
            else cout << "Hit an anoynomous object " << "at the distance of " << hitDistance << endl;
        }
        else cout << "Miss!\n";
        hitObject = nullptr;
    }

    static float lastTime = 0;
    float deltaTime = FrameTime::GetTime() - lastTime;
    if (deltaTime > 1) {
        // 最基本的敌人攻击
        enemyActions();

        // 呼吸回血
        breathRecover();

        // 结算
        if (this->playerHP <= 0) {
            cout << "You died\n";
            // 应该输出什么东西
        }
        lastTime += deltaTime;
    }
    

    
}



bool Scene::canCameraMove() {
    return true;
}

void Scene::enemyActions() {
  
    //static unsigned int cnt = 0;
   
    vec3 playerPos = mainCamera->transform->GetPosition();
    //cout << "Test " << cnt << endl;

    for (auto enemy : this->gameObjects) {
        if (enemy->isDamagable()) {
            vec3 enemyPos = enemy->transform->GetPosition();
            vec3 enemyFacing = enemy->transform->GetRotation();
            vec3 shootDir = playerPos - enemyPos;

            float hitDistance = INFINITY;
            float distance = length(shootDir);

            for (auto iter : this->gameObjects) {
                if (iter == enemy || !iter->isHitable()) continue; // 跳过所有不可以射击的物体与自己
                float temp = iter->isHit(vec4(shootDir,1), vec4(enemyPos,1));
                if (temp < hitDistance) {
                    hitDistance = temp;
                }
            }

            if (hitDistance > distance && distance < 5) {
                // 命中玩家
                cout << "Enemy object " + enemy->name << " hits the player at the distacne of " << distance << endl;
                //enemy->transform->SetRotation(vec3(shootDir.x, 0, shootDir.z));
                this->playerHP -= littleGun->getDamage(distance);
                this->playerLastHit = FrameTime::GetTime();
            }
            
        }
    }
    cout << endl;
   // cnt++;
}

void Scene::breathRecover() {
    float cur = FrameTime::GetTime();
    float delta = cur - playerLastHit;
    static float linear = 0.7, quadratic = 1.8, e = 15;
    if (delta > 4) {
        this->playerHP += e - e / (1 + linear * delta + quadratic * delta * delta);
    }
    if (this->playerHP > 100) this->playerHP = 100;
    cout << "Player HP: " << playerHP << endl;
}

int Scene::GetPlayerHP() const { return playerHP; }