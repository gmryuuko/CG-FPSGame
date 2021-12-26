#pragma once

#ifndef OBJLOADER_H
#define OBJLOADER_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Model.h"
#include "Material.h"

namespace ObjLoader {

    Model* LoadModel(const std::string& path);
    bool WriteModel(const Model& model);

}



#endif // OBJLOADER_H