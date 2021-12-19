
#include <iostream>

#include "Input.h"
#include "FrameTime.h"

namespace Input {

GLFWwindow* window;
enum class KeyState { Up, Down };

KeyState keys[512] = {KeyState::Up};
KeyState mouse[10] = {KeyState::Up}; 
float keyTime[512] = {0};
bool mouseStat[10] = {false};

bool GetKey(int keyCode) { return keys[keyCode] == KeyState::Down; }

bool GetKeyDown(int keyCode) {
    return keys[keyCode] == KeyState::Down && keyTime[keyCode] == FrameTime::GetTime();
}

bool GetMouseButtonDown(int keyCode) {
    bool res = (mouse[keyCode] == KeyState::Down && mouseStat[keyCode] == true);
    mouseStat[keyCode] = false;
    return res;
}

bool GetKeyUp(int keyCode) { return keys[keyCode] == KeyState::Up && keyTime[keyCode] == FrameTime::GetTime(); }

void GetCursor(double& xpos, double& ypos) {
    if (window == nullptr) return;
    glfwGetCursorPos(window, &xpos, &ypos);
}

void SetWindow(GLFWwindow* _window) {
    window = _window;
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
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

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    static bool isPressed = false;
    if (action == GLFW_PRESS) {
        switch (button){
        case GLFW_MOUSE_BUTTON_LEFT:
        case GLFW_MOUSE_BUTTON_MIDDLE:
        case GLFW_MOUSE_BUTTON_RIGHT:
            mouse[button] = KeyState::Down;
            mouseStat[button] = true;
            break;
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
        case GLFW_MOUSE_BUTTON_MIDDLE:
        case GLFW_MOUSE_BUTTON_RIGHT:
            mouse[button] = KeyState::Up;
            mouseStat[button] = false;
            break;
        }
    }
    return;
}

}  // namespace