#include "Input.h"

Input::Input(GLFWwindow *window) {
    this->window = window;
}

bool Input::GetKey(int keyCode) {
    return glfwGetKey(window, keyCode) == GLFW_PRESS;
}

void Input::GetCursor(double& xpos, double& ypos) {
    glfwGetCursorPos(window, &xpos, &ypos);
}