#include"SoftRender/Object.h"

Object::Object() = default;
Object::~Object() = default;
Object::Object(const Object & obj) {
	mesh = obj.mesh;
	material = obj.material;
}
Object::Object(const Mesh &m, const Material& mat) {
	mesh = m;
	material = mat;
}

Object & Object::operator =(const Object& obj) {
	if (&obj == this)
		return *this;
	mesh = obj.mesh;
	material = obj.material;
	return *this;
}