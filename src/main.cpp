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

using namespace std;

Scene& SetSceneManually();

int main() {

    GLFWwindow* window = Graphic::CreateWindow("FPS", 1920, 1080);
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

    int texture_units;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &texture_units);
    std::cout << "Max vertex texture image units: " << texture_units << std::endl;

    std::cout << "Start rendering." << std::endl;

    while (!Graphic::Closed()) {
        // new frame, clear
        Graphic::SwapFrame();
        Graphic::Clear();

        // process input
        Graphic::ProcessInput();
        scene.ProcessInput();

        // render
        Graphic::RenderScene(scene);

    }

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
    // nanosuit
//    auto nanosuit = new GameObject(Resource::GetModel("nanosuit/nanosuit.obj"));
//    nanosuit->transform->SetScale(glm::vec3(1.0 / 10));
//    scene.gameObjects.emplace_back(nanosuit);
    // earth
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++) {
//            for (int k = 0; k < 3; k++) {
//                auto earth = new GameObject(Resource::GetModel("earth/Earth 2K.obj"));
//                earth->transform->SetPosition(glm::vec3(i, j + 4, k));
//                earth->transform->SetScale(glm::vec3(1.0 / 10));
//                scene.gameObjects.emplace_back(earth);
//            }
//        }
//    }
    // cottage
    auto cottage = new GameObject(Resource::GetModel("cottage/cottage_obj.obj"));
    cottage->transform->SetScale(glm::vec3(1.0 / 10));
    scene.gameObjects.emplace_back(cottage);

    // cube
    auto cube = new GameObject(Resource::GetModel("brick/cube.obj"));
    cube->transform->SetPosition(glm::vec3(0, 0, 3));
    scene.gameObjects.emplace_back(cube);
    // ground
    auto ground = new GameObject(Resource::GetModel("stone/ground.obj"));
    scene.gameObjects.emplace_back(ground);
    // light cube
    auto lightCube = new GameObject(Resource::GetModel("cube/light_cube.obj"));
    lightCube->transform->SetPosition(glm::vec3(5, 5, 5));
    lightCube->isLight = true;
    scene.gameObjects.emplace_back(lightCube);

    // light
    // direction
    auto dirLight = new Light::DirLight{ glm::vec3(-1, -1, -1),
                                         glm::vec3(0.2, 0.2, 0.2),
                                         glm::vec3(0.7, 0.7, 0.7),
                                         glm::vec3(1, 1, 1) };
    scene.dirLights.emplace_back(dirLight);
//    scene.dirLights.emplace_back(dirLight);
    // point
//    auto pointLight = new Light::PointLight{
//        sun->transform->GetPosition(),
//        glm::vec3(0.2, 0.2, 0.2),
//        glm::vec3(0.8, 0.8, 0.8),
//        glm::vec3(1, 1, 1),
//        0, 0, 0
//    };
    // scene.pointLights.emplace_back(pointLight);
    // SolarSystem solarSystem;

    return scene;
}

void TestCalcTBN() {
    Vertex a{
            glm::vec3(-1, 1, 0),
            glm::vec3(0),
            glm::vec2(0, 1)
    };
    Vertex b {
            glm::vec3(-1, -1, 0),
            glm::vec3(0),
            glm::vec2(0, 0)
    };
    Vertex c {
            glm::vec3(1, -1, 0),
            glm::vec3(0),
            glm::vec2(1, 0)
    };

    ObjLoader::CalcTBN(a, b, c);

    cout << glm::to_string(a.tangent) << endl
         << glm::to_string(a.bitangent) << endl
         << glm::to_string(a.normal) << endl;
}