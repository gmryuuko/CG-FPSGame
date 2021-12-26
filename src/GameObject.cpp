#include "GameObject.h"
#include "Graphic.h"

GameObject::GameObject(Model* model) {
	this->model = model;
	this->transform = new Transform();
	this->transform->gameObject = this;
}

void GameObject::Draw(const Shader& objShader, const Shader& lightShader, bool drawLight) {
	if (this->HP > 0) {
		const Shader& shader = this->isLight ? lightShader : objShader;

		shader.Use();
		shader.SetMat4(Graphic::UNIFORM_MODEL_MATRIX, transform->GetModelMatrix());
		if (model != nullptr && (!this->isLight || drawLight))
			model->Draw(shader);

		for (auto& tr : transform->children) {
			tr->gameObject->Draw(objShader, lightShader);
		}
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
	if (this->hitable == false || this->HP <= 0) return INFINITY;
	glm::mat4 inverseModelMatrix = this->transform->GetInverseModelMatrix();
	viewDir = inverseModelMatrix * viewDir;
	viewPos = inverseModelMatrix * viewPos;

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
	return dist*this->transform->GetScale().x;
}

bool GameObject::isInside(vec4& modelPos) {
	if (this->HP <= 0) return false;
	vec4 pos = this->transform->GetInverseModelMatrix() * modelPos;

	divideByW(pos);
	
	for (auto iter : hitboxes) {
		if (iter->isInside(pos)) {
			modelPos = this->transform->GetModelMatrix() * pos;
			divideByW(modelPos);
			return true;
		}
	}
	return false;
}


bool GameObject::isHitable() {
	return hitable && HP > 0;
}
bool GameObject::isDamagable() {
	return hitable && damagable && HP > 0;
}