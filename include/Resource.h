#pragma once

#ifndef RESOURCE_H
#define RESOURCE_H

#include <unordered_map>
#include <string>

#include "Texture.h"

using namespace std;

class Resource {
private:
    unordered_map<string, Texture*> textures;
    unordered_map<string, Mesh*> meshes;


public:
    Texture* GetTexture(const string& path);
    Mesh* GetMesh(const string& path);

};

#endif