#include "GameObject.h"
#include "Graphic.h"

GameObject::GameObject(Model* model) {
	this->model = model;
	this->transform = new Transform();
	this->transform->gameObject = this;
}

void GameObject::Draw(const Shader& objShader, const Shader& lightShader) {
	const Shader& shader = this->isLight ? lightShader : objShader;
	
	shader.Use();
	shader.SetMat4(Graphic::UNIFORM_MODEL_MATRIX, transform->GetModelMatrix());
	if (model != nullptr)
		model->Draw(shader);

	for (auto& tr : transform->children) {
		tr->gameObject->Draw(objShader, lightShader);
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