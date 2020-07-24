#include "SoftRender\graphic.h"

//Class FrameBuffer

int cpu = 3;

FrameBuffer::~FrameBuffer()=default;

FrameBuffer::FrameBuffer(const int& w = 1280, const int& h = 720) {
	Width = w;
	Height = h;
	//RGBA
	colorBuffer.resize(w * h * 4, 0);
	depthBuffer.resize(w * h, 1.0f);
	
}

void FrameBuffer::Resize(const int& w, const int& h) {
	Width = w;
	Height = h;
	colorBuffer.resize(w * h * 4, 0);
	depthBuffer.resize(w * h, 1.0f);
}

void FrameBuffer::ClearColorBuffer(const glm::vec4& color) {
	float* p = colorBuffer.data();
	for (int i = 0; i < Width * Height * 4; i += 4) {
		*(p + i) = color.r;
		*(p + i + 1) = color.g;
		*(p + i + 2) = color.b;
		*(p + i + 3) = color.a;
		
	}

	/*
	int length = Width * Height * 4;
	for (int i = 0; i < length; i+=4) {
		colorBuffer[i] = 255;
		colorBuffer[i + 1] = 0;
		colorBuffer[i + 2] = 0;
		colorBuffer[i + 3] = 0;
	}*/
}

void FrameBuffer::ClearDepthBuffer() {
	int size = Width * Height;
	float* p = depthBuffer.data();
	for (int i = 0; i < size; i++) {
		*(p + i) = 1.0f;
	}
}
void FrameBuffer::WritePoint(const int x, const int y, const glm::vec4 color, SDL_Renderer* gRenderer) {
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return;
	int xy = (y*Width +x);

	colorBuffer[xy * 4] = color.r;
	colorBuffer[xy * 4 + 1] = color.g;
	colorBuffer[xy * 4 + 2] = color.b;
	colorBuffer[xy * 4 + 3] = color.a;
	//SDL_SetRenderDrawColor(gRenderer, colorBuffer[xy * 4], colorBuffer[xy * 4 + 1], colorBuffer[xy * 4 + 2], colorBuffer[xy * 4 + 3]);
	//SDLDrawPixel(x,y);
}

float FrameBuffer::GetDepth(const int& x, const int& y){
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return 1.0f;
	
	return *(depthBuffer.data() + y* Width + x);
	
}

void FrameBuffer::WriteDepth(const int& x, const int& y, const float& depth){
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return;
	int pos = y * Width + x;
	
	float* p = depthBuffer.data();
	*(p + pos) = depth;
	
}

//Class Vertex

Vertex::Vertex() = default;

Vertex::~Vertex() = default;

Vertex::Vertex(
	const glm::vec4& _pos,
	const glm::vec4& _color,
	const glm::vec2& _tex,
	const glm::vec3& _normal
) :
	position(_pos), color(_color), texcoord(_tex), normal(_normal) {}

Vertex::Vertex(
	const glm::vec3& _pos,
	const glm::vec4& _color,
	const glm::vec2& _tex,
	const glm::vec3& _normal 
) :
	position(_pos, 1.0f), color(_color), texcoord(_tex), normal(_normal) {}

Vertex::Vertex(const Vertex& v):
	position(v.position), color(v.color), texcoord(v.texcoord), normal(v.normal) {}

//Class V2F

V2F::V2F() = default;
V2F::~V2F() = default;

V2F::V2F(
	const glm::vec4& _wPos,
	const glm::vec4& _pPos,
	const glm::vec4& _color,
	const glm::vec2& _tex,
	const glm::vec3& _normal
) :
	worldPos(_wPos), windowPos(_pPos), color(_color), texcoord(_tex), normal(_normal) {}

V2F::V2F(const V2F& v) :
	worldPos(v.worldPos), windowPos(v.windowPos), color(v.color), texcoord(v.texcoord), normal(v.normal),Z(v.Z) {}

V2F V2F::lerp(const V2F& v1, const V2F& v2, const float& factor) {
V2F result;
result.windowPos = Lerp(v1.windowPos, v2.windowPos, factor);
result.worldPos = Lerp(v1.worldPos, v2.worldPos, factor);
result.color = Lerp(v1.color, v2.color, factor);
result.normal = Lerp(v1.normal, v2.normal, factor);
result.texcoord = Lerp(v1.texcoord, v2.texcoord, factor);
result.Z = Lerp(v1.Z, v2.Z, factor);
return result;
	}


//Class Shader

Shader::Shader() {
	ModelMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ProjectMatrix = glm::mat4(1.0f);
	Viewplanes.resize(6, glm::vec4(0));
	Texture* texture;
}

Shader::~Shader() = default;

V2F Shader::VertexShader(const Vertex& a2v) {
	V2F o;
	o.worldPos = ModelMatrix * a2v.position;
	// PVM*v
	o.windowPos = ProjectMatrix * ViewMatrix * o.worldPos;

	o.Z = 1/o.windowPos.w;
	o.worldPos =o.worldPos* o.Z;
	o.color = a2v.color*o.Z;
	o.normal = a2v.normal*o.Z;
	o.texcoord = a2v.texcoord*o.Z;
	return o;
}

/*glm::vec4 Shader::FragmentShader(const V2F& v) {
	glm::vec4 color;
	color = v.color;
	return color;
}*/

void Shader::setModelMatrix(const glm::mat4& model) {
	ModelMatrix = model;
}

void Shader::setViewMatrix(const glm::mat4& view) {
	ViewMatrix = view;
}

void Shader::setProjectMatrix(const glm::mat4& project) {
	ProjectMatrix = project;
}

glm::vec4 Shader::FragmentShader(const V2F& v,Texture texture) {
	
	glm::vec4 color =texture.Sample2D(v.texcoord);
	return color;
}

void Shader::UpdateViewPlanes() {
	ViewingFrustumPlanes(Viewplanes, ProjectMatrix * ViewMatrix);
}






void PerspectiveDivision(V2F& v) {
	
	v.windowPos /= v.windowPos.w;
	v.windowPos.w = 1.0f;
	v.windowPos.z = (v.windowPos.z +1.0f ) /2;
}


void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3, int i) {
    V2F left, right, top;
    left = v1.windowPos.x > v2.windowPos.x ? v2 : v1;
    right = v1.windowPos.x > v2.windowPos.x ? v1 : v2;
    top = v3;
    left.windowPos.x = int(left.windowPos.x);
    int dy = top.windowPos.y - left.windowPos.y;
    int nowY = top.windowPos.y;
    //�������²�ֵ
	switch (i){

		case 0: {
			nowY -= i;
			for (int i = dy; i >= 0; i -= cpu) {
				if (i < 0) {
					break;
				}
				float weight = 0;
				if (dy != 0) {
					weight = (float)i / dy;
				}
				V2F newLeft = V2F::lerp(left, top, weight);
				V2F newRight = V2F::lerp(right, top, weight);
				newLeft.windowPos.x = int(newLeft.windowPos.x);
				newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
				newLeft.windowPos.y = newRight.windowPos.y = nowY;
				ScanLine(newLeft, newRight);
				nowY -= cpu;
				
			}
			break;
		}
		case 1:{
			nowY -= i;
	for (int i = dy; i >= 0; i -= cpu) {
		if (i < 0) {
			break;
		}
		float weight = 0;
		if (dy != 0) {
			weight = (float)i / dy;
		}
		V2F newLeft = V2F::lerp(left, top, weight);
		V2F newRight = V2F::lerp(right, top, weight);
		newLeft.windowPos.x = int(newLeft.windowPos.x);
		newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
		newLeft.windowPos.y = newRight.windowPos.y = nowY;
		ScanLine(newLeft, newRight);
		nowY -= cpu;
		
	   }
	break;
	 }
		case 2: {
			nowY -= i;
			for (int i = dy; i >= 0; i -= cpu) {
				if (i < 0) {
					break;
				}
				float weight = 0;
				if (dy != 0) {
					weight = (float)i / dy;
				}
				V2F newLeft = V2F::lerp(left, top, weight);
				V2F newRight = V2F::lerp(right, top, weight);
				newLeft.windowPos.x = int(newLeft.windowPos.x);
				newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
				newLeft.windowPos.y = newRight.windowPos.y = nowY;
				ScanLine(newLeft, newRight);
				nowY -= cpu;

			}
			break;

		}
		case 3: {
			nowY -= i;
			for (int i = dy; i >= 0; i -= cpu) {
				if (i < 0) {
					break;
				}
				float weight = 0;
				if (dy != 0) {
					weight = (float)i / dy;
				}
				V2F newLeft = V2F::lerp(left, top, weight);
				V2F newRight = V2F::lerp(right, top, weight);
				newLeft.windowPos.x = int(newLeft.windowPos.x);
				newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
				newLeft.windowPos.y = newRight.windowPos.y = nowY;
				ScanLine(newLeft, newRight);
				nowY -= cpu;

			}
			break;
		}
		case 4: {
			nowY -= i;
			for (int i = dy; i >= 0; i -= cpu) {
				if (i < 0) {
					break;
				}
				float weight = 0;
				if (dy != 0) {
					weight = (float)i / dy;
				}
				V2F newLeft = V2F::lerp(left, top, weight);
				V2F newRight = V2F::lerp(right, top, weight);
				newLeft.windowPos.x = int(newLeft.windowPos.x);
				newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
				newLeft.windowPos.y = newRight.windowPos.y = nowY;
				ScanLine(newLeft, newRight);
				nowY -= cpu;

			}
			break;
		}
		case 5: {
			nowY -= i;
			for (int i = dy; i >= 0; i -= cpu) {
				if (i < 0) {
					break;
				}
				float weight = 0;
				if (dy != 0) {
					weight = (float)i / dy;
				}
				V2F newLeft = V2F::lerp(left, top, weight);
				V2F newRight = V2F::lerp(right, top, weight);
				newLeft.windowPos.x = int(newLeft.windowPos.x);
				newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
				newLeft.windowPos.y = newRight.windowPos.y = nowY;
				ScanLine(newLeft, newRight);
				nowY -= cpu;

			}
			break;
		}
		case 6: {
			nowY -= i;
			for (int i = dy; i >= 0; i -= cpu) {
				if (i < 0) {
					break;
				}
				float weight = 0;
				if (dy != 0) {
					weight = (float)i / dy;
				}
				V2F newLeft = V2F::lerp(left, top, weight);
				V2F newRight = V2F::lerp(right, top, weight);
				newLeft.windowPos.x = int(newLeft.windowPos.x);
				newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
				newLeft.windowPos.y = newRight.windowPos.y = nowY;
				ScanLine(newLeft, newRight);
				nowY -= cpu;

			}
			break;
		}
		case 7: {
			nowY -= i;
			for (int i = dy; i >= 0; i -= cpu) {
				if (i < 0) {
					break;
				}
				float weight = 0;
				if (dy != 0) {
					weight = (float)i / dy;
				}
				V2F newLeft = V2F::lerp(left, top, weight);
				V2F newRight = V2F::lerp(right, top, weight);
				newLeft.windowPos.x = int(newLeft.windowPos.x);
				newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
				newLeft.windowPos.y = newRight.windowPos.y = nowY;
				ScanLine(newLeft, newRight);
				nowY -= cpu;

			}
			break;
		}
	}
}
  


void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3, int i) {
    V2F left, right, bottom;
    left = v1.windowPos.x > v2.windowPos.x ? v2 : v1;
    right = v1.windowPos.x > v2.windowPos.x ? v1 : v2;
    bottom = v3;
    int dy = left.windowPos.y - bottom.windowPos.y;
    int nowY = left.windowPos.y;

    //�������²�ֵ
	switch (i) {
	case 0: {
		
		nowY -= i;
			for (int i = 0; i < dy; i += cpu) {
				if (i > dy) {
					break;
				}
				float weight = 0;
				if (dy != 0) {
					weight = (float)i / dy;
				}
				V2F newLeft = V2F::lerp(left, bottom, weight);
				V2F newRight = V2F::lerp(right, bottom, weight);
				newLeft.windowPos.x = int(newLeft.windowPos.x);
				newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
				newLeft.windowPos.y = newRight.windowPos.y = nowY;
				ScanLine(newLeft, newRight);
				nowY -= cpu;
			}
			break;
		}
	case 1:{
			nowY-=i;
			for (int i = 0; i < dy; i += cpu) {
				if (i > dy) {
					break;
				}
				float weight = 0;
				if (dy != 0) {
					weight = (float)i / dy;
				}
				V2F newLeft = V2F::lerp(left, bottom, weight);
				V2F newRight = V2F::lerp(right, bottom, weight);
				newLeft.windowPos.x = int(newLeft.windowPos.x);
				newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
				newLeft.windowPos.y = newRight.windowPos.y = nowY;
				ScanLine(newLeft, newRight);
				nowY -= cpu;
			}
			break;
		}
	case 2: {
		nowY-=i;
		for (int i = 0; i < dy; i += cpu) {
			if (i > dy) {
				break;
			}
			float weight = 0;
			if (dy != 0) {
				weight = (float)i / dy;
			}
			V2F newLeft = V2F::lerp(left, bottom, weight);
			V2F newRight = V2F::lerp(right, bottom, weight);
			newLeft.windowPos.x = int(newLeft.windowPos.x);
			newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
			newLeft.windowPos.y = newRight.windowPos.y = nowY;
			ScanLine(newLeft, newRight);
			nowY -= cpu;
		}
		break;

	}
	case 3: {
		nowY -=i;
		for (int i = 0; i < dy; i += cpu) {
			if (i > dy) {
				break;
			}
			float weight = 0;
			if (dy != 0) {
				weight = (float)i / dy;
			}
			V2F newLeft = V2F::lerp(left, bottom, weight);
			V2F newRight = V2F::lerp(right, bottom, weight);
			newLeft.windowPos.x = int(newLeft.windowPos.x);
			newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
			newLeft.windowPos.y = newRight.windowPos.y = nowY;
			ScanLine(newLeft, newRight);
			nowY -= cpu;
		}
		break;

	}
	case 4: {
		nowY -=i;
		for (int i = 0; i < dy; i += cpu) {
			if (i > dy) {
				break;
			}
			float weight = 0;
			if (dy != 0) {
				weight = (float)i / dy;
			}
			V2F newLeft = V2F::lerp(left, bottom, weight);
			V2F newRight = V2F::lerp(right, bottom, weight);
			newLeft.windowPos.x = int(newLeft.windowPos.x);
			newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
			newLeft.windowPos.y = newRight.windowPos.y = nowY;
			ScanLine(newLeft, newRight);
			nowY -= cpu;
		}
		break;

	}
	case 5: {
		nowY-= i;
		for (int i = 0; i < dy; i += cpu) {
			if (i > dy) {
				break;
			}
			float weight = 0;
			if (dy != 0) {
				weight = (float)i / dy;
			}
			V2F newLeft = V2F::lerp(left, bottom, weight);
			V2F newRight = V2F::lerp(right, bottom, weight);
			newLeft.windowPos.x = int(newLeft.windowPos.x);
			newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
			newLeft.windowPos.y = newRight.windowPos.y = nowY;
			ScanLine(newLeft, newRight);
			nowY -= cpu;
		}
		break;

	}
	case 6: {
		nowY -= i;
		for (int i = 0; i < dy; i += cpu) {
			if (i > dy) {
				break;
			}
			float weight = 0;
			if (dy != 0) {
				weight = (float)i / dy;
			}
			V2F newLeft = V2F::lerp(left, bottom, weight);
			V2F newRight = V2F::lerp(right, bottom, weight);
			newLeft.windowPos.x = int(newLeft.windowPos.x);
			newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
			newLeft.windowPos.y = newRight.windowPos.y = nowY;
			ScanLine(newLeft, newRight);
			nowY -= cpu;
		}
		break;

	}
	case 7: {
		nowY -= i;
		for (int i = 0; i < dy; i += cpu) {
			if (i > dy) {
				break;
			}
			float weight = 0;
			if (dy != 0) {
				weight = (float)i / dy;
			}
			V2F newLeft = V2F::lerp(left, bottom, weight);
			V2F newRight = V2F::lerp(right, bottom, weight);
			newLeft.windowPos.x = int(newLeft.windowPos.x);
			newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
			newLeft.windowPos.y = newRight.windowPos.y = nowY;
			ScanLine(newLeft, newRight);
			nowY -= cpu;
		}
		break;

	}

	}
	}



void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3, int i) {

    std::vector<V2F> arr = { v1,v2,v3 };
    if (arr[0].windowPos.y > arr[1].windowPos.y) {
        V2F tmp = arr[0];
        arr[0] = arr[1];
        arr[1] = tmp;
    }
    if (arr[1].windowPos.y > arr[2].windowPos.y) {
        V2F tmp = arr[1];
        arr[1] = arr[2];
        arr[2] = tmp;
    }
    if (arr[0].windowPos.y > arr[1].windowPos.y) {
        V2F tmp = arr[0];
        arr[0] = arr[1];
        arr[1] = tmp;
    }
    //arr[0] ��������  arr[2]��������
    //�м���������ȣ��ǵ�������
    if (arr[1].windowPos.y == arr[2].windowPos.y) {
        DownTriangle(arr[1], arr[2], arr[0],i);
    }//��������
    else if (arr[1].windowPos.y == arr[0].windowPos.y) {
        UpTriangle(arr[1], arr[0], arr[2],i);
    }
    else {
        //��ֵ����м�������Ǹ��㣬����Ϊ�����µ�������
        float weight = (arr[2].windowPos.y - arr[1].windowPos.y) / (arr[2].windowPos.y - arr[0].windowPos.y);
        V2F newEdge = V2F::lerp(arr[2], arr[0], weight);
        UpTriangle(arr[1], newEdge, arr[2],i);
        DownTriangle(arr[1], newEdge, arr[0],i);

    }
}