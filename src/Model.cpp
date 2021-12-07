#include "Model.h"

using namespace std;

Model::Model(vector<Mesh* >* meshes, const string& path) {
    this->meshes = meshes;
    this->path = path;
}

void Model::Draw(const Shader& shader) {
    for (auto& mesh : *meshes) {
        mesh->Draw(shader);
    }
}