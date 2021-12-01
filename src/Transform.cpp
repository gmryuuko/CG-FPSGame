#include "Transform.h"

using namespace std;
using namespace glm;

Transform::Transform(vec3 position, vec3 rotation, vec3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	SetDirty();
}

void Transform::UpdateModelMatrix() {
	// 位移矩阵
	mat4 tr = translate(mat4(1), position);
	// 旋转矩阵，内旋顺序Z-Y-X，相当于外旋顺序X-Y-Z
	mat4 rx = rotate(mat4(1), radians(rotation.x), vec3(1, 0, 0));
	mat4 ry = rotate(mat4(1), radians(rotation.y), vec3(0, 1, 0));
	mat4 rz = rotate(mat4(1), radians(rotation.z), vec3(0, 0, 1));
	mat4 ro = rz * ry * rx;
	// 缩放矩阵
	mat4 sc = glm::scale(mat4(1), scale);
	// model matrix，缩放->旋转->位移
	modelMatrix = tr * ro * sc * mat4(1);
	// local 坐标轴
	axisX = ro * vec4(1, 0, 0, 0);
	axisY = ro * vec4(0, 1, 0, 0);
	axisZ = ro * vec4(0, 0, 1, 0);

	if (parent != nullptr) {
		modelMatrix = parent->GetModelMatrix() * modelMatrix;
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
	this->parent = parent;
	parent->children.emplace_back(this);
	SetDirty();
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
	position += distance;
	SetDirty();
}

void Transform::RotateX(float pitch) {
	rotation.x += pitch;
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