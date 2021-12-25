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

using namespace std;

Scene& readFromXML();

sceneReader sr("../resources/scenes/scene.xml");

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


    Scene& scene = readFromXML();

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

Scene& readFromXML() {
    Scene& scene = *(new Scene());

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

    

    return scene;
}
