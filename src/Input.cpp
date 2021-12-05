#include "Input.h"

#include <iostream>
#include "FrameTime.h"

GLFWwindow* Input::window = nullptr;
Input::KeyState Input::keys[512] = { KeyState::Up };
float Input::keyTime[512] = { 0 };


bool Input::GetKey(int keyCode) {
    return keys[keyCode] == KeyState::Down;
}

bool Input::GetKeyDown(int keyCode) {
    return keys[keyCode] == KeyState::Down && keyTime[keyCode] == FrameTime::GetTime();
}

bool Input::GetKeyUp(int keyCode) {
    return keys[keyCode] == KeyState::Up && keyTime[keyCode] == FrameTime::GetTime();
}

void Input::GetCursor(double& xpos, double& ypos) {
    if (window == nullptr) return ;
    glfwGetCursorPos(window, &xpos, &ypos);
}

void Input::SetWindow(GLFWwindow* _window) {
    window = _window;
    glfwSetKeyCallback(window, KeyCallback);
}

void Input::KeyCallback(GLFWwindow* window, int keyCode, int scanCode, int action, int mods) {
    // std::cout << keyCode << ' ' << scanCode << ' ' << action << ' ' << mods << std::endl;
    if (keyCode < 0) return; // ignore unknown key

    if (action == GLFW_PRESS) {
        keys[keyCode] = KeyState::Down;
        keyTime[keyCode] = FrameTime::GetTime();
    }
    else if (action == GLFW_RELEASE) {
        keys[keyCode] = KeyState::Up;
        keyTime[keyCode] = FrameTime::GetTime();
    }
}

