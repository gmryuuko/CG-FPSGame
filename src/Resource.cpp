#include "Resource.h"
#include "obj_loader.h"

using namespace std;

unordered_map<string, Mesh*> Resource::meshes;

Mesh* Resource::GetMesh(const string& path) {
	if (meshes.find(path) != meshes.end())
		return meshes[path];

	Mesh* mesh = ObjLoader::Load(path);
	meshes[path] = mesh;
	return mesh;
}