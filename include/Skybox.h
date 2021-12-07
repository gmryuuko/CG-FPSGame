#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>

#include "Shader.h"

class Skybox {
private:
    static const float vertices[108];
    unsigned int texId, vao, vbo;

public:
    // px, nx, py, ny, pz, nz
    Skybox(std::vector<std::string> path, const std::string& directory = "../resources/textures");
    void Draw(const Shader& shader);
};

#endif