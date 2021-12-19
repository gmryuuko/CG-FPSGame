#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Graphic.h"
#include "Transform.h"
#include "FrameTime.h"
#include "Resource.h"
#include "Model.h"
#include "Light.h"
#include "SolarSystem.h"
#include "ObjLoader.h"
#include "Gui.h"

using namespace std;

Scene& SetSceneManually();

int main() {

    GLFWwindow* window = Graphic::CreateWindow("FPS", 1920, 1080);
    Gui::InitGui(window);
    // GLFWwindow* window = Graphic::CreateWindow("FPS", 800, 600);

    if (window == nullptr) {
        cout << "Create window failed!" << endl;
        return 0;
    }
    else {
        cout << "window created" << endl;
    }

    Graphic::SetClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Graphic::SetClearColor(glm::vec4(0.0, 0.0, 0.0, 1));
    Graphic::SetVSync(1); // v sync
    Graphic::SetCursorMode(GLFW_CURSOR_DISABLED);

    // ----------------------------------------------------------------
    // 手动设置场景，暂未实现从文件读取场景
    Scene& scene = SetSceneManually();
    // -----------------------------------------------------------------

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::cout << "Start rendering." << std::endl;
    while (!Graphic::Closed()) {
        // new frame, clear
        Graphic::SwapFrame();
        Graphic::Clear();
        Gui::NewFrame();
        Gui::ShowSettings(scene);

        // process input
        Graphic::ProcessInput();
        if (!Graphic::InputBlocked()) {
            scene.ProcessInput();
        }

        // render
        Graphic::RenderScene(scene);
        Gui::Render();

    }


    glfwDestroyWindow(window);
    glfwTerminate();

	return 0;
}

Scene& SetSceneManually() {
    Scene& scene = *(new Scene());

    // camera
    scene.mainCamera = new Camera();
    scene.mainCamera->transform->SetPosition(glm::vec3(0, 2, 5));

    // skybox
    scene.skybox = new Skybox(vector<string>{
            "cubemap/universe/1k_px.jpg",
            "cubemap/universe/1k_nx.jpg",
            "cubemap/universe/1k_py.jpg",
            "cubemap/universe/1k_ny.jpg",
            "cubemap/universe/1k_pz.jpg",
            "cubemap/universe/1k_nz.jpg",
    });

    // objects
    // sun
//    auto sun = new GameObject(Resource::GetModel("sun/Sun.obj"));
//    sun->transform->SetScale(glm::vec3(1.5 / 20000));
//    sun->transform->SetPosition(glm::vec3(0, 8, 0));
//    sun->isLight = true;
//    scene.gameObjects.emplace_back(sun);
     //nanosuit
    auto nanosuit = new GameObject(Resource::GetModel("nanosuit/nanosuit.obj"));
    nanosuit->transform->SetScale(glm::vec3(1.0 / 10));
    nanosuit->transform->SetPosition(glm::vec3(5, 0, 5));
    nanosuit->transform->SetRotation(glm::vec3(90, 0, 0));
    scene.gameObjects.emplace_back(nanosuit);
    // earth
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                auto earth = new GameObject(Resource::GetModel("earth/Earth 2K.obj"));
                earth->transform->SetPosition(glm::vec3(i, j + 4, k));
                earth->transform->SetScale(glm::vec3(1.0 / 10));
                scene.gameObjects.emplace_back(earth);
            }
        }
    }
    // cottage
    auto cottage = new GameObject(Resource::GetModel("cottage/cottage_obj.obj"));
    cottage->transform->SetScale(glm::vec3(1.0 / 10));
    scene.gameObjects.emplace_back(cottage);

    // cube
    auto cube = new GameObject(Resource::GetModel("brick/cube.obj"));
    cube->transform->SetPosition(glm::vec3(0, 0, 3));
    cube->hitboxes.push_back(new Hitbox(glm::vec3(0, 0, 0), 2, 2, 2));
    cube->name = "brick";
    scene.gameObjects.emplace_back(cube);
    // ground
    auto ground = new GameObject(Resource::GetModel("stone/ground.obj"));
    ground->transform->SetPosition(glm::vec3(0, 0.01, 0));
    scene.gameObjects.emplace_back(ground);
    // light cube
    auto lightCube = new GameObject(Resource::GetModel("cube/light_cube.obj"));
    lightCube->transform->SetPosition(glm::vec3(5, 5, 5));
    lightCube->isLight = true;
    scene.gameObjects.emplace_back(lightCube);
    // container
    auto container = new GameObject(Resource::GetModel("container/Container.obj"));
    container->transform->SetScale(glm::vec3(1.0 / 300));
    scene.gameObjects.emplace_back(container);
    // tower
    auto tower = new GameObject(Resource::GetModel("watch_tower/wooden_watch_tower2.obj"));
    scene.gameObjects.emplace_back(tower);
    // rock
    //auto rock = new GameObject(Resource::GetModel("rock/Rock1.obj"));
    //scene.gameObjects.emplace_back(rock);
    // city
    //auto city = new GameObject(Resource::GetModel("OrganodronCity/Organodron_City.obj"));
    //city->transform->SetScale(glm::vec3(1.0 / 100));
    //scene.gameObjects.emplace_back(city);

    // light
    // direction
    auto dirLight = new Light::DirLight{ 
        glm::vec3(10, 10, 10),
        glm::vec3(-1, -1, -1),
        glm::vec3(0.1, 0.1, 0.1),
        glm::vec3(0.8, 0.7, 0.6),
        glm::vec3(1, 1, 1) 
    };
    scene.dirLights.emplace_back(dirLight);
//    scene.dirLights.emplace_back(dirLight);
    // point
    auto pointLight = new Light::PointLight{
        lightCube->transform->GetPosition(),
        glm::vec3(0.2, 0.2, 0.2),
        glm::vec3(0.8, 0.8, 0.8),
        glm::vec3(1, 1, 1),
        0, 0, 0
    };
    // spot
    auto spotLight = new Light::SpotLight{
        glm::vec3(-5, 5, -5),
        glm::vec3(1, -1, 1),
        glm::cos(glm::radians(12.5f)),
        glm::cos(glm::radians(17.5f)),
        glm::vec3(0),
        glm::vec3(0.8, 0.8, 0.8),
        glm::vec3(1, 1, 1),
        0, 0, 0
    };
    auto flashLight = new Light::SpotLight{
        glm::vec3(-5, 5, -5),
        glm::vec3(0.1, -1, 0.1),
        glm::cos(glm::radians(12.5f)),
        glm::cos(glm::radians(17.5f)),
        glm::vec3(0),
        glm::vec3(0.8, 0.8, 0.8),
        glm::vec3(1, 1, 1),
        0, 0, 0
    };

    scene.spotLights.emplace_back(spotLight);
    scene.spotLights.emplace_back(flashLight);

    scene.mainCamera->BindSpotLight(flashLight);

     //scene.pointLights.emplace_back(pointLight);
    // SolarSystem solarSystem;

    return scene;
}
