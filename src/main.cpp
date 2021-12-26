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
#include "ObjLoader.h"
#include "Gui.h"
#include "sceneReader.h"
#include "sceneSaver.h"
#include "ParticleSystem.h"

using namespace std;

int main() {

    GLFWwindow* window = Graphic::gCreateWindow("FPS", 1920, 1080);
    Gui::InitGui(window);

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


    ParticleSystem flame(5000);
    Scene& scene = readFromXML("../resources/scenes/scene.xml");
    scene.flame = &flame;
    scene.flameTransfrom = new Transform();
    scene.flameTransfrom->SetPosition(glm::vec3(7.2, -0.1, -7.25));
    scene.flameTransfrom->SetScale(glm::vec3(1.0 / 20));

    // auto gun = new GameObject(Resource::GetModel("M24/M24_R_High_Poly_Version_obj.obj"));
    auto gun = new GameObject(Resource::GetModel("M9/Beretta_M9.obj"));
    Resource::GetModel("M24/M24_R_High_Poly_Version_obj.obj");
    gun->transform->SetPosition(vec3(0.2, -0.3, -0.7));
    gun->transform->SetRotation(vec3(0, 180, 90));
    gun->transform->SetScale(vec3(0.01));
    
    scene.mainCamera->BindGun(gun);
    scene.gameObjects.emplace_back(gun);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::cout << "Start rendering." << std::endl;

    while (!Graphic::Closed()) {
        // new frame, clear
        Graphic::SwapFrame();
        Graphic::Clear();
        Gui::NewFrame();
        Gui::ShowSettings(scene);

        // process input
        Graphic::ProcessInput(scene);
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


