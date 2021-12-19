#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Input {

void SetWindow(GLFWwindow* window);
bool GetKey(int keyCode);
bool GetKeyUp(int keyCode);
bool GetKeyDown(int keyCode);
bool GetMouseButtonDown(int keyCode);
void GetCursor(double& xpos, double& ypos);
void KeyCallback(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};  // namespace Input

#endif