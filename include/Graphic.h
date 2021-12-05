#pragma once

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>

#include "Scene.h"
#include "Camera.h"
#include "GameObject.h"
#include "Shader.h"

//class GameObject;

class Graphic {
private:
    static GLFWwindow* window;
    static unsigned int scrWidth, scrHeight;
    static unsigned int clearBit;
    static glm::vec4 clearColor;
    static Shader *mainShader;
    static void InitShader();
    static void FrameBufferSizeCallback(GLFWwindow* window, int scrWidth, int scrHeight);

public:
    static const std::string UNIFORM_MODEL_MATRIX;
    static const std::string UNIFORM_VIEW_MATRIX;
    static const std::string UNIFORM_PROJECTION_MATRIX;
    // 创建窗口
    static GLFWwindow* CreateWindow(const std::string& title, unsigned int scrWidth, unsigned int scrHeight);

    /* 
        设置Cursor模式 
        GLFW_CURSOR_NORMAL
        GLFW_CURSOR_HIDDEN
        GLFW_CURSOR_DISABLED
    */
    static void SetCursorMode(int value);
    static int Closed();
    static void SwapFrame();
    static void Clear();
    static void SetClear(unsigned int value);
    static void RemoveClear(unsigned int value);
    static void SetClearColor(glm::vec4 color);
    static void RenderScene(Scene& scene);
    // 0: 关闭, 1: 打开, 2: 一半...
    static void SetVSync(unsigned int value);
};

#endif