#pragma once

#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mesh
{
private:
	unsigned int vao, vbo, ebo;
	std::vector<Vertex>* vertices;
	std::vector<unsigned int>* indices;
	void Init();
	~Mesh();

public:
	Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);
	void Draw(const Shader& shader);
};


#endif // !MESH_H