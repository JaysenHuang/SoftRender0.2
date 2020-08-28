#include "SoftRender/draw.h"



void setModelMatrix(const glm::mat4& model) {
	shader.setModelMatrix(model);
}
void setViewMatrix(const glm::mat4& view) {
	shader.setViewMatrix(view);
}
void setProjectMatrix(const glm::mat4& project) {
	shader.setProjectMatrix(project);
}
bool FaceCulling(const glm::vec4 &v1,const glm::vec4 &v2,const glm::vec4 &v3) {
	glm::vec3 tmp1 = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	glm::vec3 tmp2 = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
	glm::vec3 normal = glm::normalize(glm::cross(tmp1, tmp2));

	return glm::dot(normal, glm::vec3(0, 0, 1))>= 0;  //返回1则证明为正面
}

bool ViewCull(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3) {
	
	/*glm::vec3 minPoint, maxPoint;
	minPoint.x =  glm::min(v1.x, glm::min(v2.x, v3.x));
	minPoint.y =  glm::min(v1.y, glm::min(v2.y, v3.y));
	minPoint.z =  glm::min(v1.z, glm::min(v2.z, v3.z));
	maxPoint.x =  glm::max(v1.x, glm::max(v2.x, v3.x));
	maxPoint.y =  glm::max(v1.y, glm::max(v2.y, v3.y));
	maxPoint.z =  glm::max(v1.z, glm::max(v2.z, v3.z));
	
	if (!Point2Plane(shader.Viewplanes[4], minPoint) && !Point2Plane(shader.Viewplanes[4], maxPoint)){
	//	std::cout << "1" << std::endl;
		return false;
	} //near
	if (!Point2Plane(shader.Viewplanes[5], minPoint) && !Point2Plane(shader.Viewplanes[5], maxPoint)) {
	//	std::cout << "2" << std::endl;
		return false;
	} //far
	if (!Point2Plane(shader.Viewplanes[0], minPoint) && !Point2Plane(shader.Viewplanes[0], maxPoint)) {
	//	std::cout << "3" << std::endl;
		return false;
	} //zuo
	if (!Point2Plane(shader.Viewplanes[1], minPoint) && !Point2Plane(shader.Viewplanes[1], maxPoint)) {
		//std::cout << "4" << std::endl;
		return false;
	} //you
	if (!Point2Plane(shader.Viewplanes[2], minPoint) && !Point2Plane(shader.Viewplanes[2], maxPoint)) {
		//std::cout << "5" << std::endl;
		return false;
	} //shang
	if (!Point2Plane(shader.Viewplanes[3], minPoint) && !Point2Plane(shader.Viewplanes[3], maxPoint)) {
		//std::cout << "6" << std::endl;
		return false;
	} //xia
	return true;*/
	if (v1.w <= camera.N && v2.w <= camera.N && v3.w <= camera.N)
		return false;
	if (v1.w >= camera.F && v2.w <= camera.F && v3.w <= camera.F)
		return false;
	if (v1.x <= v1.w || v1.y <= v1.w || v1.z <= v1.w)
		return true;
	if (v2.x <= v2.w || v2.y <= v2.w || v2.z <= v2.w)
		return true;
	if (v3.x <= v3.w || v3.y <= v3.w || v3.z <= v3.w)
		return true;
	return false;
}

void DrawObject(Object &obj) {
	
	if (obj.mesh.EBO.empty()) {
		
	return;
		
	}
	
	currentMat =&obj.material;

#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < obj.mesh.EBO.size(); i += 3) {
		Vertex p1, p2, p3;
		p1 = obj.mesh.VBO[obj.mesh.EBO[i]];
		p2 = obj.mesh.VBO[obj.mesh.EBO[i + 1]];
		p3 = obj.mesh.VBO[obj.mesh.EBO[i + 2]];



		V2F	v1, v2, v3;
		v1 = currentMat->shader->VertexShader(p1);
		v2 = currentMat->shader->VertexShader(p2);
		v3 = currentMat->shader->VertexShader(p3);


		//做透视除法 变换到NDC
		if (!ViewCull(v1.windowPos, v2.windowPos, v3.windowPos)) {
			continue;
		}
		PerspectiveDivision(v1);
		PerspectiveDivision(v2);
		PerspectiveDivision(v3);

		if (!FaceCulling(v1.windowPos, v2.windowPos, v3.windowPos)) {
			continue;
		}

		v1.windowPos = ViewPortMatrix * v1.windowPos;
		v2.windowPos = ViewPortMatrix * v2.windowPos;
		v3.windowPos = ViewPortMatrix * v3.windowPos;

		if (renderMod == Line) {
			DrawLine(v1, v2);
			DrawLine(v2, v3);
			DrawLine(v3, v1);
		}
		else {
			ScanLineTriangle(v1, v2, v3, 1);
		}		
	}
}

void DrawModel(Model &model) {
 
	for (int i = 0; i < model.objects.size(); i++) {
	
		DrawObject(model.objects[i]);		
	}
}

void DrawLine(const V2F& from, const V2F& to)
{
	int dx = to.windowPos.x - from.windowPos.x;
	int dy = to.windowPos.y - from.windowPos.y;
	int Xstep = 1, Ystep = 1;
	if (dx < 0)
	{
		Xstep = -1;
		dx = -dx;
	}
	if (dy < 0)
	{
		Ystep = -1;
		dy = -dy;
	}
	int currentX = from.windowPos.x;
	int currentY = from.windowPos.y;
	V2F tmp;
	//斜率小于1
	if (dy <= dx)
	{
		int P = 2 * dy - dx;
		for (int i = 0; i <= dx; ++i)
		{
			tmp = V2F::lerp(from, to, ((float)(i) / dx));
			FrontBuffer->WritePoint(currentX, currentY, glm::vec4(255, 0, 0, 0));
			currentX += Xstep;
			if (P <= 0)
				P += 2 * dy;
			else
			{
				currentY += Ystep;
				P += 2 * (dy - dx);
			}
		}
	}
	//斜率大于1，利用对称性画
	else
	{
		int P = 2 * dx - dy;
		for (int i = 0; i <= dy; ++i)
		{
			tmp = V2F::lerp(from, to, ((float)(i) / dy));
			FrontBuffer->WritePoint(currentX, currentY, glm::vec4(255, 0, 0, 0));
			currentY += Ystep;
			if (P <= 0)
				P += 2 * dx;
			else
			{
				currentX += Xstep;
				P -= 2 * (dy - dx);
			}
		}
	}
}

