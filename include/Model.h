#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include "Mesh.h"
#include "Shader.h"

class Model {
private:
    std::vector<Mesh* >* meshes;
    std::string path;

public:
    Model(std::vector<Mesh* >* meshes, const std::string& path = "");
    void Draw(const Shader& shader);
};

#endif // MODEL_H