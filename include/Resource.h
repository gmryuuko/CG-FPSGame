#pragma once

#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <unordered_map>

#include "Mesh.h"
#include "Texture.h"

namespace Resource {
// std::unordered_map<std::string, Texture*> textures;

Texture* GetTexture(const std::string& path);
Mesh* GetMesh(const std::string& path);

}  // namespace Resource

#endif