#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

#include "GameObject.h"

class GameObject;

/*
	rotation相关部分写的很烂，建议不要用roll角（绕Z轴旋转），两个自由度应该也够了
	scale建议XYZ均等缩放
*/
class Transform {

private:
	// 根据position, rotation, scale计算的model矩阵
	glm::mat4 modelMatrix = glm::mat4(1);
	// modelMatrix是否需要重新计算
	bool dirty = true;
	// 位移
	glm::vec3 position;
	// rotation = (pitch, yaw, roll)
	glm::vec3 rotation;
	// 沿X, Y, Z方向的缩放
	glm::vec3 scale;
	// 更新model矩阵
	void UpdateModelMatrix();
	// 
	void CheckDirty();
	void SetDirty();
	// axis
	glm::vec3 axisX, axisY, axisZ;


public:

	GameObject* gameObject = nullptr;
	Transform* parent = nullptr;
	std::vector<Transform*> children;

	Transform(const Transform& transform);
	Transform(glm::vec3 position = glm::vec3(1), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
	// 设置position
	void SetPosition(const glm::vec3& position);
	// 设置rotation
	void SetRotation(const glm::vec3& rotation);
	// 设置scale
	void SetScale(const glm::vec3& scale);
	// 设置parent节点
	void SetParent(Transform* parent);
	void RemoveParent();
	// 向direction方向移动distance
	void Translate(const glm::vec3& direction, float distance);
	// X轴旋转角增加pitch[角度]
	void RotateX(float pitch, bool limit = false, float lower = -89, float upper = 89);
	// Y轴旋转角增加yaw[角度]
	void RotateY(float yaw);
	// Z轴旋转角增加roll[角度]
	void RotateZ(float roll);
	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;
	glm::vec3 GetAxisX();
	glm::vec3 GetAxisY();
	glm::vec3 GetAxisZ();
	glm::mat4 GetModelMatrix();
};

#endif