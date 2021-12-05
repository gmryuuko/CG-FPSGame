#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Transform.h"
#include "Mesh.h"

class Transform;

class GameObject {
private:
    Transform* transform;
    Mesh* mesh;

public:
	GameObject(Mesh* mesh);
	// 设置model矩阵，然后绘制
	void Draw(const Shader& shader);

	void SetParent(GameObject* parent);

	void RemoveParent();

	void Test();
};

#endif // !GAMEOBJECT_H
