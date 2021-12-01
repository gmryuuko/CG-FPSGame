#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Input {
private:
    GLFWwindow* window;

public:
    Input(GLFWwindow* window);
    bool GetKey(int keyCode);
    void GetCursor(double &xpos, double &ypos);
};

#endif