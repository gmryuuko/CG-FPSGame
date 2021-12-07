#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Transform.h"
#include "Model.h"

class Transform;

class GameObject {
private:
	Model* model;

public:
	bool isLight = false;
	Transform* transform;
	GameObject(Model* model);
	// 设置model矩阵，然后绘制
	void Draw(const Shader& objShader, const Shader& lightShader);

	void SetParent(GameObject* parent);

	void RemoveParent();

	void Test();
};

#endif // !GAMEOBJECT_H
