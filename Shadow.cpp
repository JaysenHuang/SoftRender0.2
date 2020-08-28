#include"SoftRender/shadow.h"

void Shadow::Fit2Scene(Object& o) {
	std::vector<glm::vec3> aabbBounds = GetSceneAABBCorners(o);
	
	float Xmin = 99999;
	float Ymin = 99999;
	float Zmin = 99999;
	float Xmax = -99999;
	float Ymax = -99999;
	float Zmax = -99999;
	for (glm::vec3 cornerPoints : aabbBounds) {
		glm::vec3 pointInLightSpace = LightSpaceCam.ViewMatrix() * glm::vec4(cornerPoints, 1);

		Xmin = glm::min(pointInLightSpace.x, Xmin);
		Xmax = glm::max(pointInLightSpace.x, Xmax);
		Ymin = glm::min(pointInLightSpace.y, Ymin);
		Ymax = glm::max(pointInLightSpace.y, Ymax);
		Zmin = glm::min(pointInLightSpace.z, Zmin);
		Zmax = glm::max(pointInLightSpace.z, Zmax);
	}
	float xsize = (Xmax - Xmin) / 2;
	float ysize = (Ymax - Ymin) / 2;
	float zsize = (Zmax - Zmin) / 2;

	LightSpaceCam.Position = glm::inverse(LightSpaceCam.ViewMatrix()) * glm::vec4((Xmin + Xmax) / 2, (Ymin + Ymax) / 2, 0,1);
	LightSpaceCam.Size = glm::max(xsize, ysize);
	LightSpaceCam.N = Zmin;
	LightSpaceCam.F = Zmax;

}
bool Shadow::IsInShadow(V2F& v) {
	return !IsVisibleToLight(v, dirLight);
}
bool Shadow::IsVisibleToLight(V2F& v, DirectionLight *L) {
	glm::vec4 LightSpacePos = LightSpaceCam.PerspectiveMatrix() * LightSpaceCam.ViewMatrix() *v.worldPos;
	LightSpacePos /= LightSpacePos.w;
	LightSpacePos.w = 1.0f;
	LightSpacePos.z = (LightSpacePos.z + 1.0f) / 2;
	LightSpacePos = ViewPortMatrix * LightSpacePos;
	//std::cout << LightSpacePos.z << std::endl;
	if (LightSpacePos.z-0.001> CurrentDepth(LightSpacePos.x, LightSpacePos.y)) {
		
		return false;
		
	}
	else {
		return true;
	}

}
Shadow::Shadow(DirectionLight* L, Object& o) {
	LightSpaceCam.Position = L->Position;
	LightSpaceCam.Front = L->Direction;
	LightSpaceCam.Right = glm::normalize(glm::cross(LightSpaceCam.Front, glm::vec3(0,1,0)));
	LightSpaceCam.Up = glm::normalize(glm::cross(LightSpaceCam.Right, LightSpaceCam.Front));
	LightSpaceCam.Aspect = (float)1280 / 720;
	LightSpaceCam.Fov = glm::radians(60.0f);
}
Shadow::~Shadow() = default;

std::vector<glm::vec3> Shadow::GetSceneAABBCorners(Object& o) {
	std::vector<glm::vec3> k;
	k.resize(o.mesh.EBO.size());
	currentMat = &o.material;
	for (int i = 0; i < o.mesh.EBO.size(); i += 3) {
	 	k.push_back(currentMat->shader->VertexShader(o.mesh.VBO[o.mesh.EBO[i]]).worldPos);
		k.push_back(currentMat->shader->VertexShader(o.mesh.VBO[o.mesh.EBO[i + 1]]).worldPos);
		k.push_back(currentMat->shader->VertexShader(o.mesh.VBO[o.mesh.EBO[i + 2]]).worldPos);		
	}
	return k;
}

float Shadow::CurrentDepth(float x ,float y) {
	float result = ShadowBuffer->GetDepth(x,y);
	return result;
}