#include "Mesh.h"
#include "Graphic.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

void Mesh::Init() {
	if (ready) return;
	ready = true;

	// generate buffer
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// bind data
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(*vertices)[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &(*indices)[0], GL_STATIC_DRAW);

	// set attribute pointer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
}

Mesh::~Mesh() {
	delete vertices;
	delete indices;
}

Mesh::Mesh() {
	vertices = nullptr;
	indices = nullptr;
	ready = false;
}

Mesh::Mesh(vector<Vertex>* vertices, vector<unsigned int>* indices) {
	this->vertices = vertices;
	this->indices = indices;
	Init();
}

void Mesh::Draw(const Shader& shader) {
	if (!ready) {
		cerr << "[Mesh] Not ready!" << endl;
		return;
	}

	// set material
	shader.Use();
	// std::cout << indices->size() << std::endl;
	shader.SetVec3(Graphic::UNIFORM_AMBIENT_COLOR, this->material.colorAmbient);
	shader.SetVec3(Graphic::UNIFORM_DIFFUSE_COLOR, this->material.colorDiffuse);
	shader.SetVec3(Graphic::UNIFORM_SPECULAR_COLOR, this->material.colorSpecular);
	shader.SetFloat(Graphic::UNIFORM_SHININESS, this->material.shininess);
	if (this->material.texDiffuse == nullptr) {
		shader.SetBool(Graphic::UNIFORM_USE_DIFFUSE_TEX, false);
	} else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->material.texDiffuse->texId);
		shader.SetBool(Graphic::UNIFORM_USE_DIFFUSE_TEX, true);
		shader.SetInt(Graphic::UNIFORM_DIFFUSE_TEX, 0);
	}
	if (this->material.texSpecular == nullptr) {
		shader.SetBool(Graphic::UNIFORM_USE_SPECULAR_TEX, false);
	} else {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->material.texSpecular->texId);
		shader.SetBool(Graphic::UNIFORM_USE_SPECULAR_TEX, true);
		shader.SetInt(Graphic::UNIFORM_SPECULAR_TEX, 1);
	}
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Init(vector<Vertex>* vertices, vector<unsigned int>* indices) {
	if (ready) {
		cerr << "[Mesh] Already initialized!" << endl;
		return;
	}

	this->vertices = vertices;
	this->indices = indices;
	Init();
}
