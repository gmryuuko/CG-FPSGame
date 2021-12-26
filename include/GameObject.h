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
public:
	Model* model;
	

public:
	std::string name;
	std::string category;
	std::string modelPath;
	std::vector<Hitbox*>hitboxes;
	bool isLight = false;
	Transform* transform;
	bool hitable = false;
	bool damagable = false;
	int HP = 100;

	GameObject(Model* model);
	// 设置model矩阵，然后绘制
	void Draw(const Shader& objShader, const Shader& lightShader, bool drawLight = true);
	void SetParent(GameObject* parent);
	void RemoveParent();
	void Test();
	float isHit(vec4 viewDir, vec4 viewPos);
	bool isInside(vec4& modelPos);
	bool isHitable();
	bool isDamagable();
};

#endif // !GAMEOBJECT_H
