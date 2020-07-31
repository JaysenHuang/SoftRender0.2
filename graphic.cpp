#include "SoftRender\graphic.h"

//Class FrameBuffer

int cpu =1;

extern std::mutex mx;

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
	char* p = colorBuffer.data();
	for (int i = 0; i < Width * Height * 4; i += 4) {
		*(p + i) = color.r;
		*(p + i + 1) = color.g;
		*(p + i + 2) = color.b;
		*(p + i + 3) = color.a;
		
	}

}

void FrameBuffer::ClearDepthBuffer() {
	int size = Width * Height;
	float* p = depthBuffer.data();
	for (int i = 0; i < size; i++) {
		*(p + i) = 1.0f;
	}
}
void FrameBuffer::WritePoint(const int x, const int y, const glm::vec4 color) {
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return;
	int xy = 4*(y*Width +x);
	int r, g, b, a;
	r = color.x * 255;
	g = color.y * 255;
	b = color.z * 255;
	a = color.w * 255;
	char* p = colorBuffer.data();
	*(p + xy) =r;
	*(p + xy + 1) = g;
	*(p + xy + 2) = b;
	*(p + xy + 3) = a;
	
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




void PerspectiveDivision(V2F& v) {
	
	v.windowPos /= v.windowPos.w;
	v.windowPos.w = 1.0f;
	v.windowPos.z = (v.windowPos.z +1.0f ) /2;
}


void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3) {
    V2F left, right, top;
    left = v1.windowPos.x > v2.windowPos.x ? v2 : v1;
    right = v1.windowPos.x > v2.windowPos.x ? v1 : v2;
    top = v3;
    left.windowPos.x = int(left.windowPos.x);
    int dy = top.windowPos.y - left.windowPos.y;
    int nowY = top.windowPos.y;
    //从上往下插值
	int i = cpu;
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
  


void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3) {
    V2F left, right, bottom;
    left = v1.windowPos.x > v2.windowPos.x ? v2 : v1;
    right = v1.windowPos.x > v2.windowPos.x ? v1 : v2;
    bottom = v3;
    int dy = left.windowPos.y - bottom.windowPos.y;
    int nowY = left.windowPos.y;
	int i = cpu;
    //从上往下插值
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



void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3) {

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
    //arr[0] 在最下面  arr[2]在最上面
    //中间跟上面的相等，是底三角形
    if (arr[1].windowPos.y == arr[2].windowPos.y) {
        DownTriangle(arr[1], arr[2], arr[0]);
    }//顶三角形
    else if (arr[1].windowPos.y == arr[0].windowPos.y) {
        UpTriangle(arr[1], arr[0], arr[2]);
    }
    else {
        //插值求出中间点对面的那个点，划分为两个新的三角形
        float weight = (arr[2].windowPos.y - arr[1].windowPos.y) / (arr[2].windowPos.y - arr[0].windowPos.y);
        V2F newEdge = V2F::lerp(arr[2], arr[0], weight);
        UpTriangle(arr[1], newEdge, arr[2]);
        DownTriangle(arr[1], newEdge, arr[0]);

    }
}


void SwapBuffer()
{
	mx.lock();
	FrameBuffer* p = FrontBuffer;
	FrontBuffer = BackBuffer;
	BackBuffer = p;
	mx.unlock();
//	std::cout << FrontBuffer << std::endl;
}