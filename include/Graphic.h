#pragma once

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>

class Graphic {
   private:
    static GLFWwindow* window;
    static void FrameBufferSizeCallback(GLFWwindow* window, int scrWidth, int scrHeight);
    static unsigned int clearBit;
    static glm::vec4 clearColor;

   public:
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
    static void NewFrame();
    static void Clear();
    static void SetClear(unsigned int value);
    static void RemoveClear(unsigned int value);
    static void SetClearColor(glm::vec4 color);
    static void RenderScene();
};

#endif