
#include <iostream>

#include "Input.h"
#include "FrameTime.h"

namespace Input {

enum class KeyState { Up, Down };
GLFWwindow* window = nullptr;
KeyState keys[512] = {KeyState::Up};
float keyTime[512] = {0};

bool GetKey(int keyCode) { return keys[keyCode] == KeyState::Down; }

bool GetKeyDown(int keyCode) {
    return keys[keyCode] == KeyState::Down && keyTime[keyCode] == FrameTime::GetTime();
}

bool GetKeyUp(int keyCode) { return keys[keyCode] == KeyState::Up && keyTime[keyCode] == FrameTime::GetTime(); }

void GetCursor(double& xpos, double& ypos) {
    if (window == nullptr) return;
    glfwGetCursorPos(window, &xpos, &ypos);
}

void SetWindow(GLFWwindow* _window) {
    window = _window;
    glfwSetKeyCallback(window, KeyCallback);
}

void KeyCallback(GLFWwindow* window, int keyCode, int scanCode, int action, int mods) {
    // std::cout << keyCode << ' ' << scanCode << ' ' << action << ' ' << mods << std::endl;
    if (keyCode < 0) return;  // ignore unknown key

    if (action == GLFW_PRESS) {
        keys[keyCode] = KeyState::Down;
        keyTime[keyCode] = FrameTime::GetTime();
    } else if (action == GLFW_RELEASE) {
        keys[keyCode] = KeyState::Up;
        keyTime[keyCode] = FrameTime::GetTime();
    }
}

}  // namespace