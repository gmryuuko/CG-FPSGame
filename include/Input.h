#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Input {
private:
    static GLFWwindow* window;
    enum class KeyState {
        Up, Down
    };
    static KeyState keys[512];
    static float keyTime[512];

public:
    // Input(GLFWwindow* window);
    static void SetWindow(GLFWwindow* window);
    static bool GetKey(int keyCode);
    static bool GetKeyUp(int keyCode);
    static bool GetKeyDown(int keyCode);
    static void GetCursor(double &xpos, double &ypos);
    static void KeyCallback(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);
    // static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif