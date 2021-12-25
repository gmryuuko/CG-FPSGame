#include "Transform.h"

#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

Transform::Transform(const Transform& transform) {
	this->position = transform.position;
	this->rotation = transform.rotation;
	this->scale = transform.scale;
	SetDirty();
}


Transform::Transform(vec3 position, vec3 rotation, vec3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	SetDirty();
}

void Transform::UpdateModelMatrix() {
	// 位移矩阵
	translateMatrix = translate(mat4(1), position);
	// 旋转矩阵，内旋顺序Z-Y-X，相当于外旋顺序X-Y-Z
	mat4 rx = rotate(mat4(1), radians(rotation.x), vec3(1, 0, 0));
	mat4 ry = rotate(mat4(1), radians(rotation.y), vec3(0, 1, 0));
	mat4 rz = rotate(mat4(1), radians(rotation.z), vec3(0, 0, 1));
	rotationMatrix = rz * ry * rx;
	// 缩放矩阵
	scaleMatrix = glm::scale(mat4(1), scale);
	// model matrix，缩放->旋转->位移
	modelMatrix = translateMatrix * rotationMatrix * scaleMatrix * mat4(1);
	// inverse model matrix
	inverseModelMatrix = glm::inverse(modelMatrix);
	// local 坐标轴
	axisX = rotationMatrix * vec4(1, 0, 0, 0);
	axisY = rotationMatrix * vec4(0, 1, 0, 0);
	axisZ = rotationMatrix * vec4(0, 0, 1, 0);

	if (parent != nullptr) {
		modelMatrix = parent->GetTranslateMatrix() * parent->GetRotationMatrix() * modelMatrix;
	}

	dirty = false;
}

void Transform::SetDirty() {
	if (!dirty) {
		dirty = true;
		for (auto child : children) {
			child->SetDirty();
		}
	}
}

void Transform::CheckDirty() {
	if (dirty) {
		UpdateModelMatrix();
	}
}

mat4 Transform::GetModelMatrix() {
	CheckDirty();
	return modelMatrix;
}

mat4 Transform::GetTranslateMatrix() {
	CheckDirty();
	return translateMatrix;
}

mat4 Transform::GetRotationMatrix() {
	CheckDirty();
	return rotationMatrix;
}

mat4 Transform::GetScaleMatrix() {
	CheckDirty();
	return scaleMatrix;
}
glm::mat4 Transform::GetInverseModelMatrix() {
	CheckDirty();
	return inverseModelMatrix;
}

void Transform::SetPosition(const vec3& position) {
	this->position = position;
	SetDirty();
}

void Transform::SetRotation(const vec3& rotation) {
	this->rotation = rotation;
	SetDirty();
}

void Transform::SetScale(const vec3& scale) {
	this->scale = scale;
	SetDirty();
}

vec3 Transform::GetPosition() const {
	return position;
}

vec3 Transform::GetRotation() const {
	return rotation;
}

vec3 Transform::GetScale() const {
	return scale;
}

void Transform::SetParent(Transform* parent) {
	if (this->parent != nullptr) {
		RemoveParent();
	}
	if (parent == nullptr) return;

	this->parent = parent;
	parent->children.emplace_back(this);
	SetDirty();
}

void Transform::RemoveParent() {
	if (parent != nullptr) {
		for (vector<Transform*>::iterator i = parent->children.begin();
			i != parent->children.end(); i++) {
			if (*i == this) {
				parent->children.erase(i);
				break;
			}
		}
		parent = nullptr;
		SetDirty();
	}
}


vec3 Transform::GetAxisX() {
	CheckDirty();
	return axisX;
}

vec3 Transform::GetAxisY() {
	CheckDirty();
	return axisY;
}

vec3 Transform::GetAxisZ() {
	CheckDirty();
	return axisZ;
}

void Transform::Translate(const vec3& direction, float distance) {
	vec3 move = normalize(direction) * distance;
	// cout << glm::to_string(move) << std::endl;
	position += move;
	SetDirty();
}

void Transform::RotateX(float pitch, bool limit, float lower, float upper) {
	rotation.x += pitch;

	if (limit) {
		if (rotation.x > upper)
			rotation.x = upper;
		if (rotation.x < lower)
			rotation.x = lower;
	}

	SetDirty();
}

void Transform::RotateY(float yaw) {
	rotation.y += yaw;
	SetDirty();
}

void Transform::RotateZ(float roll) {
	rotation.z += roll;
	SetDirty();
}