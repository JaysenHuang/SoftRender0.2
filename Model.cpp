#include"SoftRender/Model.h"

Model::Model() = default;
Model::~Model() = default;
Model::Model(const Model & model) {
	objects = model.objects;
}
Model::Model(const std::string& path) {
	LoadObj(path);
}
Model& Model::operator=(const Model& model) {
	if (&model == this)
		return *this;
	objects = model.objects;
	return *this;
}
void Model::SetMaterial(const int& id, const Material& m) {
	objects[id].material = m;
}
void Model::LoadObj(const std::string& path) {
	std::ifstream in(path);
	if (!in) {
		std::cout << "Open Obj File Failed" << std::endl;
		return;
	}
	
	std::vector<glm::vec3>vertexs;
	std::vector<glm::vec3>normal;
	std::vector<glm::vec2>texcoords;

	std::string line;

	int currentObjectNums = -1;
	bool f = false;
	while (!in.eof()) {
		
		std::getline(in, line);
		if (!line.compare(0, 3, "v  "))
		{
			if (!f) {		
				currentObjectNums++;
				Object o;
				objects.push_back(o);
				f = true;
			}
			line = line.substr(3);
			std::istringstream iss(line);
			glm::vec3 v;
			iss >> v.x;
			iss >> v.y;
			iss >> v.z;
			vertexs.push_back(v);
			continue;
		}
		if (!line.compare(0, 3, "vn ")) {
		
			line = line.substr(3);
			std::istringstream iss(line);
			glm::vec3 vn;
			iss >> vn.x;
			iss >> vn.y;
			iss >> vn.z;
			normal.push_back(vn);
			continue;
		}
		if (!line.compare(0, 3, "vt ")) {

			line = line.substr(3);
			std::istringstream iss(line);
			glm::vec3 vt;
			iss >> vt.x;
			iss >> vt.y;
			vt.y = 1 - vt.y;
			iss >> vt.z;
			texcoords.push_back(glm::vec2(vt.x,vt.y));
			continue;
		}
		if (!line.compare(0, 2, "f ")) {

			if (f) {
				f = false;
			}
			line = line.substr(2);
			std::istringstream iss(line);
			char bar;
			int vIndex, vtIndex, vnIndex;
			int offset = objects[currentObjectNums].mesh.VBO.size();
			for (int i = 0; i < 3; i++) {
				iss >> vIndex >> bar >> vtIndex >> bar >> vnIndex;
				if (vIndex > vertexs.size() || vtIndex > texcoords.size() || vnIndex > normal.size()) {
					int sb = 2;
					sb++;
				}
				Vertex vertex(vertexs[vIndex - 1], glm::vec4(1, 1, 1, 1), texcoords[vtIndex - 1], normal[vnIndex - 1]);
				objects[currentObjectNums].mesh.VBO.push_back(vertex);
				objects[currentObjectNums].mesh.EBO.push_back(offset + i);
			}
			continue;
		}
	}
	in.close();
}