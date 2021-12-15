#include "ObjLoader.h"
#include "Resource.h"

#include <unordered_map>

using namespace std;
using namespace glm;

namespace ObjLoader {

	unordered_map<string, Material>* LoadMetarial(const string& path);

	vector<string>* split(string& lineStr, char delim = ' ', bool ignEmpty = true) {
		stringstream line(lineStr);
		string str;
		vector<string>* res = new vector<string>();
		while (getline(line, str, delim)) {
			if (!ignEmpty || str.size() != 0) res->emplace_back(str);
		}
		return res;
	}

    // TODO: 这里直接忽略了obj文件中的顶点法线，直接使用面法线当作三个顶点的法线，这样做对吗？
    void CalcTBN(Vertex& a, Vertex& b, Vertex&c) {
        vec3 e1 = b.position - a.position;
        vec3 e2 = c.position - a.position;
        vec3 normal = normalize(cross(e1, e2));
        vec2 uv1 = b.texCoord - a.texCoord;
        vec2 uv2 = c.texCoord - a.texCoord;
        vec3 tangent, bitanget;

        float f = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);
        tangent.x = f * (uv2.y * e1.x - uv1.y * e2.x);
        tangent.y = f * (uv2.y * e1.y - uv1.y * e2.y);
        tangent.z = f * (uv2.y * e1.z - uv1.y * e2.z);
        tangent = normalize(tangent);
        bitanget.x = f * (-uv2.x * e1.x + uv1.x * e2.x);
        bitanget.y = f * (-uv2.x * e1.y + uv1.x * e2.y);
        bitanget.z = f * (-uv2.x * e1.z + uv1.x * e2.z);
        bitanget = normalize(bitanget);

        a.tangent = b.tangent = c.tangent = tangent;
        a.bitangent = b.bitangent = c.bitangent = bitanget;
        //a.normal = b.normal = c.normal = normal;
    };

	Model* LoadModel(const string& path) {
		auto mtls = LoadMetarial(path);
		cout << "\t.mtl read." << endl;

		ifstream fin;
		fin.open(path, ios::in);

		if (!fin.is_open()) {
			cerr << "ObjLoader: Cannot open file: \"" << path << "\" !" << endl;
		}

		bool fail = false;  // 解析失败
		int lineNum = 0;    // 当前行号
		vector<vec3> positions, normals; // v vn
		vector<vec2> texCoords;          // vt

		// 创建model失败时记得delete
		auto meshes = new vector<Mesh* >();
		vector<Vertex>* vertices = nullptr;
		vector<unsigned int>* indices = nullptr;
		Mesh* currentMesh = nullptr;

		string line;
		while (getline(fin, line)) {
			lineNum++;
			vector<string>* words = split(line);
			auto& ws = *words;

			float x, y, z;
			if (words->size() == 0 || ws[0][0] == '#') {  // 空行或注释
				continue;
			}
			else if (ws[0] == "mtllib") {  // 加载 mtl 文件

			}
			else if (ws[0] == "o" || ws[0] == "g") {  // object, group
				if (currentMesh != nullptr) {
					// set last mesh
					currentMesh->Init(vertices, indices);
					meshes->emplace_back(currentMesh);
				}
				// next mesh
				vertices = new vector<Vertex>;
				indices = new vector<unsigned int>;
				currentMesh = new Mesh();
			}
			else if (ws[0] == "s") {  // smooth

			}
			else if (ws[0] == "v") {  // Vertex
				if (ws.size() != 4) {
					fail = true;
				}
				else {
					positions.emplace_back(stof(ws[1]), stof(ws[2]), stof(ws[3]));
				}
			}
			else if (ws[0] == "vt") {  // Texture coordinates
				if (ws.size() < 3) {
					// 有的obj文件纹理坐标是三维，第三个数永远是0
					fail = true;
				}
				else {
					texCoords.emplace_back(stof(ws[1]), stof(ws[2]));
				}
			}
			else if (ws[0] == "vn") {  // normal
				if (ws.size() != 4) {
					fail = true;
				}
				else {
					normals.emplace_back(stof(ws[1]), stof(ws[2]), stof(ws[3]));
				}
			}
			else if (ws[0] == "usemtl") {  // use material
				if (ws.size() < 2) {
					fail = true;
				}
				else {
					string mtlName = ws[1];
					if (mtls->find(mtlName) == mtls->end()) {
						cout << "[ObjLoader] No material named " + mtlName << endl;
					}
					else {
						currentMesh->material = (*mtls)[mtlName];
					}
				}
			}
			else if (ws[0] == "f") {  // face
				// cout << "face" << endl;
				if (ws.size() < 4 || vertices == nullptr || indices == nullptr) {
					fail = true;
				}
				else {
					int index = vertices->size();
					for (int vi = 1; vi < ws.size(); vi++) {
						auto nums = split(ws[vi], '/', false); // 按'/'分割，不忽略空字符串
						auto& ns = *nums;
						Vertex aVertex;
						aVertex.position = positions[stoi(ns[0]) - 1];
						if (ns.size() >= 2 && ns[1].size() != 0) aVertex.texCoord = texCoords[stoi(ns[1]) - 1];
						if (ns.size() >= 3 && ns[2].size() != 0) {
                            aVertex.normal = normals[stoi(ns[2]) - 1];

                        }
						vertices->emplace_back(aVertex);
						delete nums;
					}
					// cout << index << endl;

					// 面至少有3个点
					indices->emplace_back(index);
					indices->emplace_back(index + 1);
					indices->emplace_back(index + 2);
                    CalcTBN((*vertices)[index], (*vertices)[index + 1], (*vertices)[index + 2]);

					// 如果是4个点的面，还要再加一个三角形
					if (ws.size() == 5) {
						indices->emplace_back(index + 2);
						indices->emplace_back(index + 3);
						indices->emplace_back(index);
                        CalcTBN((*vertices)[index + 2], (*vertices)[index + 3], (*vertices)[index]);
					}
				}
			}
			else if (ws[0] == "l") {  // line

			}
			else {
				fail = true;
			}

			// 不能提前break，否则这个words无法delete
			delete words;

			if (fail) break;
		}

		fin.close();

		cout << "\t.obj read." << endl;

		if (fail) {
			cerr << "[ObjLoader] Load obj file: \"" << path << "\" failed at line: " << lineNum << endl;
			cerr << line << endl;
			delete vertices;
			delete indices;
			delete currentMesh;
			for (auto m : *meshes) {
				delete m;
			}
			delete meshes;
			return nullptr;
		}


		if (currentMesh != nullptr) {
			currentMesh->Init(vertices, indices);
			meshes->emplace_back(currentMesh);
		}

		return new Model(meshes);
	}

	unordered_map<string, Material>* LoadMetarial(const string& objPath) {
		string directory = objPath.substr(0, objPath.rfind('/'));
		string mtlPath = objPath.substr(0, objPath.rfind('.')) + ".mtl";

		std::cout << "[ObjLoader] Loading..." << endl;
		cout << "\tobj: " << objPath << endl;
		cout << "\tmtl: " << mtlPath << endl;
		cout << "\tdir: " << directory << endl;

		ifstream fin;
		fin.open(mtlPath);
		if (!fin.is_open()) {
			cout << "[ObjLoader] No .mtl file, use default material." << endl;
			return new unordered_map<string, Material>;
		}

		auto& mtls = *(new unordered_map<string, Material>);
		string curMtlName = "@default_mtl";
		Material curMtl = defaultMaterial;

		bool fail = false;
		int lineNum = 0;
		string line;
		while (getline(fin, line)) {
			lineNum++;

			vector<string>* words = split(line);
			auto& ws = *words;

			if (words->size() == 0 || ws[0][0] == '#') {  // 空行或注释
				continue;
			}
			else if (ws[0] == "newmtl") {
				mtls[curMtlName] = curMtl;
				curMtlName = ws.at(1);
				curMtl = defaultMaterial;
			}
			else if (ws[0] == "Ns") {
				if (ws.size() < 2) {
					fail = true;
				}
				else {
					curMtl.shininess = stof(ws.at(1));
				}
			}
			else if (ws[0] == "Ka") {
				if (ws.size() < 4) {
					fail = true;
				}
				else {
					curMtl.colorAmbient = vec3(stof(ws.at(1)), stof(ws.at(2)), stof(ws.at(3)));
				}
			}
			else if (ws[0] == "Kd") {
				if (ws.size() < 4) {
					fail = true;
				}
				else {
					curMtl.colorDiffuse = vec3(stof(ws.at(1)), stof(ws.at(2)), stof(ws.at(3)));
				}
			}
			else if (ws[0] == "Ks") {
				if (ws.size() < 4) {
					fail = true;
				}
				else {
					curMtl.colorSpecular = vec3(stof(ws.at(1)), stof(ws.at(2)), stof(ws.at(3)));
				}
			}
			else if (ws[0] == "Ke") {
				// ignore
			}
			else if (ws[0] == "Ni") {
				// ignore
			}
			else if (ws[0] == "d") {
				// ignore
			}
			else if (ws[0] == "illum") {
				if (ws.size() < 2) {
					fail = true;
				}
				else {
					curMtl.illum = stoi(ws.at(1));
				}
			}
			else if (ws[0] == "map_Kd") {
				if (ws.size() < 2) {
					fail = true;
				}
				else {
					string texPath = ws.at(1);
					curMtl.texDiffuse = Resource::GetTexture(texPath, directory);
				}
			}
			else if (ws[0] == "map_Ks") {
				if (ws.size() < 2) {
					fail = true;
				}
				else {
					string texPath = ws.at(1);
					curMtl.texSpecular = Resource::GetTexture(texPath, directory);
				}
			} else if (ws[0] == "norm" || ws[0] == "map_norm") {
                if (ws.size() < 2) {
                    fail = true;
                }
                else {
                    string texPath = ws.at(1);
                    curMtl.texNormal = Resource::GetTexture(texPath, directory);
                }
            }

			delete words;

			if (fail) break;
 		}

		if (fail) {
			cerr << "[ObjLoader] Load mtl file: \"" << mtlPath << "\" failed at line: " << lineNum << endl;
			cerr << line << endl;
		}

		fin.close();
		
		mtls[curMtlName] = curMtl;
		return &mtls;
	}

}  // namespace ObjLoader