#include "GameObject.h"
#include "Graphic.h"

GameObject::GameObject(Mesh* mesh) {
	this->mesh = mesh;
	this->transform = new Transform();
	this->transform->gameObject = this;
}

void GameObject::Draw(const Shader& shader) {
	shader.Use();
	shader.SetMat4(Graphic::UNIFORM_MODEL_MATRIX, transform->GetModelMatrix());
	if (mesh != nullptr)
		mesh->Draw(shader);

	for (auto& tr : transform->children) {
		tr->gameObject->Draw(shader);
	}
}

void GameObject::SetParent(GameObject* parent) {
	transform->SetParent(parent->transform);
}

void GameObject::RemoveParent() {
	transform->RemoveParent();
}

void GameObject::Test() {
	std::cout << "!!!" << std::endl;
}