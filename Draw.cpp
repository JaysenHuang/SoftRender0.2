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

	return glm::dot(normal, glm::vec3(0, 0, 1))< 0;  //返回1则证明为正面
}

bool ViewCull(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3) {
	glm::vec3 minPoint, maxPoint;
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
	return true;
}

void DrawObject(Object &obj) {
	
	if (obj.mesh.EBO.empty()) {
		
	return;
		
	}
	
	Material *currentMat =&obj.material;
	
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

		PerspectiveDivision(v1);
		PerspectiveDivision(v2);
		PerspectiveDivision(v3);

		if (!ViewCull(v1.windowPos, v2.worldPos, v3.windowPos)) {
			continue;
		}
		if (!FaceCulling(v1.windowPos, v2.windowPos, v3.windowPos)) {
			continue;
		}
	
		v1.windowPos =  ViewPortMatrix * v1.windowPos;
		v2.windowPos =  ViewPortMatrix * v2.windowPos;
		v3.windowPos =  ViewPortMatrix * v3.windowPos;
		
		std::thread st0(ScanLineTriangle, v1, v2, v3, 1);
		std::thread st1(ScanLineTriangle, v1, v2, v3, 0);
		std::thread st2(ScanLineTriangle, v1, v2, v3, 2);
	// 	std::thread st3(ScanLineTriangle, v1, v2, v3, 1);
	//	std::thread st4(ScanLineTriangle, v1, v2, v3, 4);
	//	std::thread st5(ScanLineTriangle, v1, v2, v3, 5);
	//	std::thread st6(ScanLineTriangle, v1, v2, v3, 6);
		//std::thread st7(ScanLineTriangle, v1, v2, v3, 7);
		st0.join();
		st1.join();
		st2.join();
	 //  st3.join();
	 //	st4.join();
	//	st5.join();
	//	st6.join();
		//st7.join();
		//ScanLineTriangle(v1, v2, v3);


	}
}

void DrawModel(Model &model) {
	for (int i = 0; i < model.objects.size(); i++) {
		DrawObject(model.objects[i]);		
	}
}



void  Draw(SDL_Renderer* gRenderer, Shader shader, FrameBuffer FrontBuffer, const int SCREEN_WIDTH,const int SCREEN_HEIGHT) {
	
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
	
			for (int k = 0; k < SCREEN_WIDTH; k++) {
			
					if (k < 0 || k > SCREEN_WIDTH || i < 0 || i > SCREEN_HEIGHT)
						return;


					int xy = i * SCREEN_WIDTH + k;
					if (FrontBuffer.colorBuffer[xy * 4] == 75) {
						continue;
					}
					else {
						SDL_SetRenderDrawColor(gRenderer, FrontBuffer.colorBuffer[xy * 4], FrontBuffer.colorBuffer[xy * 4 + 1], FrontBuffer.colorBuffer[xy * 4 + 2], FrontBuffer.colorBuffer[xy * 4 + 3]);
						//SDL_SetRenderDrawColor(gRenderer, shader.FragmentShader(v).x, shader.FragmentShader(v).y, shader.FragmentShader(v).z, shader.FragmentShader(v).w);
						SDLDrawPixel(k, i);
					

				
			}
		}
	}
}

void Draw2(SDL_Renderer* gRenderer, Shader shader, FrameBuffer FrontBuffer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {

	for (int j = 1; j < SCREEN_HEIGHT; j+=2) {
	
		
			for (int w = 1; w < SCREEN_WIDTH; w+=2) {
			
				
				if (w < 0 || w > SCREEN_WIDTH || j < 0 || j > SCREEN_HEIGHT)
					return;


				int xy = j * SCREEN_WIDTH + w;
				if (FrontBuffer.colorBuffer[xy * 4] == 75) {
					continue;
				}
				else {
					SDL_SetRenderDrawColor(gRenderer, FrontBuffer.colorBuffer[xy * 4], FrontBuffer.colorBuffer[xy * 4 + 1], FrontBuffer.colorBuffer[xy * 4 + 2], FrontBuffer.colorBuffer[xy * 4 + 3]);
					//SDL_SetRenderDrawColor(gRenderer, shader.FragmentShader(v).x, shader.FragmentShader(v).y, shader.FragmentShader(v).z, shader.FragmentShader(v).w);
					SDLDrawPixel(w, j);
				

			}
		}
	}
}

void MyDraw(V2F &v,SDL_Renderer* gRenderer, Shader& shader, FrameBuffer& FrontBuffer, Texture texture) {
	 SDL_SetRenderDrawColor(gRenderer, 255*shader.FragmentShader(v,texture).x, 255 * shader.FragmentShader(v, texture).y, 255 * shader.FragmentShader(v, texture).z, 255 * shader.FragmentShader(v, texture).w);
	 SDLDrawPixel(v.windowPos.x, v.windowPos.y);
}