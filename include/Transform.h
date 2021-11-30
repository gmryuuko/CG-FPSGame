#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

using namespace glm;
using namespace std;

class Transform {

private:
	// 根据position, rotation, scale计算的model矩阵
	mat4 modelMatrix = mat4(1);
	// modelMatrix是否需要重新计算
	bool dirty = true;
	// 位移
	vec3 position;
	// 按Z, Y, X的顺序旋转
	vec3 rotation;
	// 沿X, Y, Z方向的缩放
	vec3 scale;
	// 更新model矩阵
	void UpdateModelMatrix();
	// 
	void SetDirty();

	Transform* parent = nullptr;
	vector<Transform*> children;

public:

	Transform(vec3 position = vec3(1), vec3 rotation = vec3(1), vec3 scale = vec3(1));
	mat4 GetModelMatrix();
	void SetPosition(const vec3& position);
	void SetRotation(const vec3& rotation);
	void SetScale(const vec3& scale);
	vec3 GetPosition() const;
	vec3 GetRotation() const;
	vec3 GetScale() const;
	void SetParent(Transform* parent);
};

#endif