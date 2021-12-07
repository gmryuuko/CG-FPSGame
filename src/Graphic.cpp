#include "Graphic.h"
#include "Input.h"
#include "FrameTime.h"

#include <iostream>

using std::string;
using namespace glm;

namespace Graphic {

void InitShader();
void FrameBufferSizeCallback(GLFWwindow* window, int scrWidth, int scrHeight);

GLFWwindow* window = nullptr;
unsigned int scrWidth = 0;
unsigned int scrHeight = 0;
unsigned int clearBit = 0;
glm::vec4 clearColor = glm::vec4(0, 0, 0, 1);
Shader* mainShader = nullptr;
Shader* lightShader = nullptr;
Shader* skyboxShader = nullptr;


GLFWwindow* CreateWindow(const std::string& title, unsigned int scrWidth, unsigned int scrHeight) {
    
    Graphic::scrWidth = scrWidth;
    Graphic::scrHeight = scrHeight;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // create window
    window = glfwCreateWindow(scrWidth, scrHeight, title.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    // callback
    glfwSetFramebufferSizeCallback(window, &FrameBufferSizeCallback);
    std::cout << "debug" << std::endl;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    // depth test
    glEnable(GL_DEPTH_TEST);
    // cull back face
    // glEnable(GL_CULL_FACE);
    // v sync
    glfwSwapInterval(1);

    Input::SetWindow(window);

    InitShader();

    return window;
}

void FrameBufferSizeCallback(GLFWwindow* window, int scrWidth, int scrHeight) {

    Graphic::scrWidth = scrWidth;
    Graphic::scrHeight = scrHeight;

    glViewport(0, 0, scrWidth, scrHeight);
}

void SetCursorMode(int value) {
    if (window == nullptr) {
        return ;
    }
    glfwSetInputMode(window, GLFW_CURSOR, value);
}

int Closed() {
    if (window == nullptr) {
        return 1;
    }
    return glfwWindowShouldClose(window);
}

void Clear() {
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(clearBit);
}


void SetClear(unsigned int value) {
    clearBit |= value;
}
void RemoveClear(unsigned int value) {
    clearBit &= ~value;
}
void SetClearColor(glm::vec4 color) {
    clearColor = color;
}

void SwapFrame() {
    FrameTime::UpdateTime();
    
    // std::cout << "New Frame" << std::endl;

    glfwSwapBuffers(window);
    glfwPollEvents();

    if (Input::GetKeyUp(GLFW_KEY_ESCAPE)) {
        std::cout << "ESC up" << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
    if (Input::GetKeyDown(GLFW_KEY_SPACE)) {
        std::cout << "Space down" << std::endl;
    }
    if (Input::GetKey(GLFW_KEY_SPACE)) {
        std::cout << "Space pressing" << std::endl;
    }
    if (Input::GetKeyUp(GLFW_KEY_SPACE)) {
        std::cout << "Space up" << std::endl;
    }
}

void SetVSync(unsigned int value) {
    glfwSwapInterval(value);
}

void InitShader() {
    mainShader = new Shader("../shader/main.vert", "../shader/main.frag");
    lightShader = new Shader("../shader/light.vert", "../shader/light.frag");
    skyboxShader = new Shader("../shader/skybox.vert", "../shader/skybox.frag");
}

void RenderScene(Scene& scene) {
    if (scene.mainCamera == nullptr) return;

    // Set camera
    mat4 projection = perspective(glm::radians(scene.mainCamera->zoom), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
    mainShader->Use();
    mainShader->SetMat4(UNIFORM_PROJECTION_MATRIX, projection);
    mainShader->SetMat4(UNIFORM_VIEW_MATRIX, scene.mainCamera->GetViewMatrix());
    mainShader->SetVec3("viewPos", scene.mainCamera->transform->GetPosition());
    lightShader->Use();
    lightShader->SetMat4(UNIFORM_PROJECTION_MATRIX, projection);
    lightShader->SetMat4(UNIFORM_VIEW_MATRIX, scene.mainCamera->GetViewMatrix());

    // set light
    mainShader->Use();
    mainShader->SetInt("nPointLights", scene.pointLights.size());
    for (int i = 0; i < scene.pointLights.size(); i++) {
        Light::SetPointLight(*mainShader, i, scene.pointLights[i]);
    }

    // render object
    for (auto& obj : scene.gameObjects) {
        obj->Draw(*mainShader, *lightShader);
        // obj->Test();
    }

    // render skybox
    if (scene.skybox != nullptr) {
        skyboxShader->Use();
        skyboxShader->SetMat4(UNIFORM_VIEW_MATRIX, mat4(mat3(scene.mainCamera->GetViewMatrix()))); // 消除位移
        skyboxShader->SetMat4(UNIFORM_PROJECTION_MATRIX, projection);
        scene.skybox->Draw(*skyboxShader);
    }
}

} // namespace 