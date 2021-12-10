#include "Light.h"

#include <string>

using namespace std;

namespace Light {
    void SetPointLight(Shader& shader, int index, PointLight *light) {
        shader.Use();
        string alight = "pointLights[" + to_string(index) + "].";
        shader.SetVec3(alight + "position", light->position);
        shader.SetVec3(alight + "ambient", light->ambient);
        shader.SetVec3(alight + "diffuse", light->diffuse);
        shader.SetVec3(alight + "specular", light->specular);
    }
    void SetDirLight(Shader& shader, int index, DirLight* light) {
        shader.Use();
        string alight = "dirLights[" + to_string(index) + "].";
        shader.SetVec3(alight + "direction", light->direction);
        shader.SetVec3(alight + "ambient", light->ambient);
        shader.SetVec3(alight + "diffuse", light->diffuse);
        shader.SetVec3(alight + "specular", light->specular);
    }
}