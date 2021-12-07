#pragma once

#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <unordered_map>

#include "Model.h"
#include "Texture.h"

namespace Resource {

    Texture* GetTexture(const std::string& path, const std::string& directory = "../resources/textures");
    Model* GetModel(const std::string& path, const std::string& directory = "../resources/models");

}  // namespace Resource

#endif