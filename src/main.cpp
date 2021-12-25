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


    Scene& scene = readFromXML("../resources/scenes/scene.xml");

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


