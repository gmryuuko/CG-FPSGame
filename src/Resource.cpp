#include "Resource.h"

#include "obj_loader.h"

using namespace std;

namespace Resource {

unordered_map<string, Mesh*> meshes;

Mesh* GetMesh(const string& path) {
    if (meshes.find(path) != meshes.end()) return meshes[path];

    Mesh* mesh = ObjLoader::Load(path);
    meshes[path] = mesh;
    return mesh;
}

}  // namespace Resource