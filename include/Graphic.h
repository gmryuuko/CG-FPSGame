#pragma once

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>

#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"
#include "Shader.h"

// class GameObject;

namespace Graphic {

// transform
const std::string UNIFORM_MODEL_MATRIX = "model";
const std::string UNIFORM_VIEW_MATRIX = "view";
const std::string UNIFORM_PROJECTION_MATRIX = "projection";

// material
const std::string UNIFORM_AMBIENT_COLOR = "colorAmbient";
const std::string UNIFORM_DIFFUSE_COLOR = "colorDiffuse";
const std::string UNIFORM_SPECULAR_COLOR = "colorSpecular";
const std::string UNIFORM_SHININESS = "shininess";

// texture
const std::string UNIFORM_USE_DIFFUSE_TEX = "useTexDiff";
const std::string UNIFORM_USE_SPECULAR_TEX = "useTexSpec";
const std::string UNIFORM_DIFFUSE_TEX = "texDiffuse";
const std::string UNIFORM_SPECULAR_TEX = "texSpecular";


// 创建窗口
GLFWwindow* CreateWindow(const std::string& title, unsigned int scrWidth, unsigned int scrHeight);

/*
    设置Cursor模式
    GLFW_CURSOR_NORMAL
    GLFW_CURSOR_HIDDEN
    GLFW_CURSOR_DISABLED
*/
void SetCursorMode(int value);
int Closed();
void SwapFrame();
void Clear();
void SetClear(unsigned int value);
void RemoveClear(unsigned int value);
void SetClearColor(glm::vec4 color);
void RenderScene(Scene& scene);
// 0: 关闭, 1: 打开, 2: 一半...
void SetVSync(unsigned int value);
};  // namespace Graphic

#endif