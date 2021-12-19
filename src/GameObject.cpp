#include "GameObject.h"
#include "Graphic.h"

GameObject::GameObject(Model* model) {
	this->model = model;
	this->transform = new Transform();
	this->transform->gameObject = this;
}

void GameObject::Draw(const Shader& objShader, const Shader& lightShader, bool drawLight) {
	const Shader& shader = this->isLight ? lightShader : objShader;
	
	shader.Use();
	shader.SetMat4(Graphic::UNIFORM_MODEL_MATRIX, transform->GetModelMatrix());
	if (model != nullptr && (!this->isLight || drawLight))
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

float GameObject::isHit(vec4 viewDir, vec4 viewPos) {
	viewDir = inverse(this->transform->GetModelMatrix()) * viewDir;
	viewPos = inverse(this->transform->GetModelMatrix()) * viewPos;

	divideByW(viewDir);
	divideByW(viewPos);


	float dist = INFINITY;
	//cout << "The view position is:" << viewPos.x << ", " << viewPos.y << ", " << viewPos.z << endl;
	for (auto iter : hitboxes) {
		//cout << "!!!" << endl;
		float temp = iter->isHit(vec3(viewPos), vec3(viewDir));
		if (temp < dist) {
			dist = temp;
		}
	}
	return dist;
}