#include "Graphic.h"

GLFWwindow* Graphic::window = nullptr;
unsigned int Graphic::clearBit = 0;
glm::vec4 Graphic::clearColor = glm::vec4(0, 0, 0, 1);

GLFWwindow* Graphic::CreateWindow(const std::string& title, unsigned int scrWidth, unsigned int scrHeight) {
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

    return window;
}

void Graphic::FrameBufferSizeCallback(GLFWwindow* window, int scrWidth, int scrHeight) {
    glViewport(0, 0, scrWidth, scrHeight);
}

void Graphic::SetCursorMode(int value) {
    if (window == nullptr) {
        return ;
    }
    glfwSetInputMode(window, GLFW_CURSOR, value);
}

int Graphic::Closed() {
    if (window == nullptr) {
        return 1;
    }
    return glfwWindowShouldClose(window);
}

void Graphic::Clear() {
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(clearBit);
}


void Graphic::SetClear(unsigned int value) {
    clearBit |= value;
}
void Graphic::RemoveClear(unsigned int value) {
    clearBit &= ~value;
}
void Graphic::SetClearColor(glm::vec4 color) {
    clearColor = color;
}

void Graphic::NewFrame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}