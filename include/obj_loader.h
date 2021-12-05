#pragma once

#ifndef OBJLOADER_H
#define OBJLOADER_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Mesh.h"

namespace ObjLoader
{

	using namespace std;
	using namespace glm;



	vector<string>* split(string& lineStr, char delim = ' ', bool ignEmpty = true);

	Mesh* Load(const string& path)
	{
		ifstream fin;
		fin.open(path, ios::in);

		if (!fin.is_open())
		{
			cerr << "ObjLoader: Cannot open file: \"" << path << "\" !" << endl;
		}


		
		bool fail = false; // 解析失败
		int lineNum = 0; // 当前行号
		vector<vec3> positions, normals;
		vector<vec2> texCoords;
		auto vertices = new vector<Vertex>();
		auto indices = new vector<unsigned int>();

		string line;
		while (getline(fin, line)) {
			lineNum++;
			vector<string>* words = split(line);
			auto& ws = *words;
			
			float x, y, z;
			if (words->size() == 0 || ws[0][0] == '#') { // 空行或注释
				continue;
			}
			else if (ws[0] == "mtllib") { // 加载 mtl 文件
				
			}
			else if (ws[0] == "o") { // New object

			}
			else if (ws[0] == "g") { // group

			}
			else if (ws[0] == "s") { // smooth

			}
			else if (ws[0] == "v") { // Vertex
				if (ws.size() != 4) {
					fail = true;
					break;
				}
				positions.emplace_back(stof(ws[1]), stof(ws[2]), stof(ws[3]));
			}
			else if (ws[0] == "vt") { // Texture coordinates
				if (ws.size() != 3) {
					fail = true;
					break;
				}
				texCoords.emplace_back(stof(ws[1]), stof(ws[2]));
			}
			else if (ws[0] == "vn") { // normal
				if (ws.size() != 4) {
					fail = true;
					break;
				}
				normals.emplace_back(stof(ws[1]), stof(ws[2]), stof(ws[3]));
			}
			else if (ws[0] == "usemtl") { // use material

			}
			else if (ws[0] == "f") { // face
				if (ws.size() < 4) {
					fail = true;
					break;
				}
				int index = vertices->size();
				for (int vi = 1; vi < ws.size(); vi++) {
					auto nums = split(ws[vi], '/', false);
					auto& ns = *nums;
					Vertex aVertex;
					aVertex.position = positions[stoi(ns[0]) - 1];
					if (ns.size() >= 1 && ns[1].size() != 0) 
						aVertex.texCoord = texCoords[stoi(ns[1]) - 1];
					if (ns.size() >= 2 && ns[2].size() != 0) 
						aVertex.normal = normals[stoi(ns[2]) - 1];
					vertices->emplace_back(aVertex);
					delete nums;
				}

				indices->emplace_back(index);
				indices->emplace_back(index + 1);
				indices->emplace_back(index + 2);
				
				if (ws.size() == 5) { // 4个点
					indices->emplace_back(index + 2);
					indices->emplace_back(index + 3);
					indices->emplace_back(index);
				}
			}
			else if (ws[0] == "l") { // line

			}
			else {
				fail = true;
				break;
			}

			delete words;
		}

		fin.close();

		if (fail) {
			cerr << "ObjLoader: Load obj file: \"" << path << "\" failed at line: " << lineNum << endl;
			delete vertices;
			delete indices;
			return nullptr;
		}
		return new Mesh(vertices, indices);
	}


	vector<string>* split(string& lineStr, char delim, bool ignEmpty) {
		stringstream line(lineStr);
		string str;
		vector<string>* res = new vector<string>();
		while (getline(line, str, delim)) {
			if (!ignEmpty || str.size() != 0)
				res->emplace_back(str);
		}
		return res;
	}

}



#endif // OBJLOADER_H