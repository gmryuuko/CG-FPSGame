#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Hitbox.h"
#include "Transform.h"
#include "Model.h"

#include "glm/glm.hpp"
#include "glm/matrix.hpp"

#include <vector>

class Transform;

class GameObject {
private:
	Model* model;

public:
	std::string name;
	std::string category;
	std::string modelPath;
	std::vector<Hitbox*>hitboxes;
	bool isLight = false;
	Transform* transform;

	GameObject(Model* model);
	// 设置model矩阵，然后绘制
	void Draw(const Shader& objShader, const Shader& lightShader, bool drawLight = true);

	void SetParent(GameObject* parent);

	void RemoveParent();

	void Test();

	float isHit(vec4 viewDir, vec4 viewPos);
};

#endif // !GAMEOBJECT_H
