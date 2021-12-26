#pragma once

#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mesh
{
private:
	unsigned int vao = 0, vbo = 0, ebo = 0;
	// 是否调用Init()初始化了？
	bool ready = false;
	void Init();

public:	
	std::vector<Vertex>* vertices;
	std::vector<unsigned int>* indices;

	~Mesh();
	Mesh();
	Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);
	Material material = defaultMaterial;
	void Init(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);
	void Draw(const Shader& shader);
};


#endif // !MESH_H