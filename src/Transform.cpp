#include "Transform.h"

Transform::Transform(vec3 position, vec3 rotation, vec3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	SetDirty();
}

void Transform::UpdateModelMatrix() {
	mat4 tr = translate(mat4(1), position);
	mat4 rx = rotate(mat4(1), radians(rotation.x), vec3(1, 0, 0));
	mat4 ry = rotate(mat4(1), radians(rotation.y), vec3(0, 1, 0));
	mat4 rz = rotate(mat4(1), radians(rotation.z), vec3(0, 0, 1));
	mat4 sc = glm::scale(mat4(1), scale);
	modelMatrix = tr * rx * ry * rz * sc * mat4(1);

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

mat4 Transform::GetModelMatrix() {
	if (dirty) {
		UpdateModelMatrix();
	}
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
