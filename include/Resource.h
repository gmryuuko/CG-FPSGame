#pragma once

#ifndef RESOURCE_H
#define RESOURCE_H

#include <unordered_map>
#include <string>

#include "Texture.h"
#include "Mesh.h"



static class Resource {
private:
    // static std::unordered_map<std::string, Texture*> textures;
    static std::unordered_map<std::string, Mesh*> meshes;


public:
    static Texture* GetTexture(const std::string& path);
    static Mesh* GetMesh(const std::string& path);

};

#endif