#include "SoftRender\graphic.h"
#include "SoftRender\setting.h"
#include "SoftRender\draw.h"
#include<windows.h>
#include <ctime>

#include"SoftRender/Light.h"
#include"SoftRender/Model.h"
#include<gdiplus.h>
//Screen dimension constants

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
SDL_Window* gWindow = NULL;
SDL_Event event;
//The window renderer
SDL_Renderer* gRenderer = NULL;
Shader shader;
Material* currentMat;
FrameBuffer* FrontBuffer;
DirectionLight *dirLight =NULL;
PointLight* pointLight =NULL;
SpotLight* spotLight=NULL;
glm::mat4 ViewPortMatrix = glm::mat4(1.0f);
std::mutex mx;
Camera camera(glm::vec3(0,0,10),glm::vec3(0,1,0),glm::vec3(0,0,-1), glm::radians(60.0f),SCREEN_WIDTH,SCREEN_HEIGHT);
FrameBuffer* BackBuffer;
Texture texture("C:\\Users\\jiasheng.huang\\Documents\\GitHub\\MySoftRender\\SoftRender\\Assets\\car.tga");
Texture texture2("C:\\Users\\jiasheng.huang\\Documents\\GitHub\\MySoftRender\\SoftRender\\Assets\\wood02.jpg");



int fps =0;
void ShowFps(SDL_Window* gWindow) {
    while (1) {
        Sleep(1000);
       
        std::string text = "HuangBao's SoftRender fps:" + std::to_string(fps);
        SDL_SetWindowTitle(gWindow, text.c_str());
        fps = 0;
    }
}
void SDLDrawPixel(int x, int y)
{
    mx.lock();
    SDL_RenderDrawPoint(gRenderer, x, SCREEN_HEIGHT - 1 - y);
    mx.unlock();
}
void ScanLine(const V2F& left, const V2F& right) {
    int length = right.windowPos.x - left.windowPos.x;
#pragma omp parallel for schedule(dynamic) 
    for (int i = 0; i < length; i++) {
        V2F v = V2F::lerp(left, right, (float)i / length);
        v.windowPos.x = left.windowPos.x + i;
        v.windowPos.y = left.windowPos.y;



        float depth = FrontBuffer->GetDepth(v.windowPos.x, v.windowPos.y);

        //  FrontBuffer.ClearDepthBuffer(v.windowPos.x, v.windowPos.y);
        if (v.windowPos.z < depth) {
            float z = v.Z;
            v.worldPos /= z;
            v.texcoord /= z;
            v.color /= z;
            v.normal /= z;
            FrontBuffer->WritePoint(v.windowPos.x, v.windowPos.y,shader.FragmentShader(v));
           // MyDraw(v);
             //SDL_SetRenderDrawColor(gRenderer, shader.FragmentShader(v, texture).x, shader.FragmentShader(v, texture).y, shader.FragmentShader(v, texture).z, shader.FragmentShader(v, texture).w);
              //SDLDrawPixel(v.windowPos.x, v.windowPos.y);
            FrontBuffer->WriteDepth(v.windowPos.x, v.windowPos.y, v.windowPos.z);
        }


    }

}

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("HuangBao's SoftRender", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
        }
    }

    return success;
}

void close()
{
    //Destroy window    
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* args[])
{
    //Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        {
            //Main loop flag
            bool quit = false;
            
            //Event handler
            SDL_Event e;
            //加载贴图
         //   texture.LoadTexture("GO");
            FrontBuffer = new FrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
            BackBuffer = new FrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
          /*  int width, height, nrChannels;
            unsigned char* data = stbi_load("C:\\Users\\jiasheng.huang\\Desktop\\\MySoftRender\\IMG_0093.PNG", &width, &height, &nrChannels, 0);
            if (data)
            {
                std::
                << "load texture" << std::endl;
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);
            */
          

            //2D
			ViewPortMatrix = GetViewPortMatrix(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

            //3D
        
          
          // setViewMatrix(GetViewMatrix(glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)));
          // setProjectMatrix(GetPerspectiveMatrix(glm::radians(60.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.3f, 500));
		    FrontBuffer->Resize(SCREEN_WIDTH, SCREEN_HEIGHT);

        //绘制点
	    //	Vertex V1(glm::vec3(-0.5, -0.5, 0), glm::vec4(255, 0, 0, 0), glm::vec2(0,0), glm::vec3(0, 0,0));
		//	Vertex V2(glm::vec3(0.5, 0.5, 0), glm::vec4(0, 255, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0));
		//	Vertex V3(glm::vec3(-0.5, 0.5, 0), glm::vec4(0,0, 255, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0));       

        //绘制box
       //   Mesh Box = CreateBox(glm::vec3(0.0, 0.0, 0.0), 1.5f);
            Material mat;
            mat.SetShader(&shader);
            Material mat2;
            mat2.SetShader(&shader);
            mat2.SetTexture(&texture2);
            mat.SetTexture(&texture);
            mat.Color = glm::vec4(1, 1, 1,1);
          //  Material mat2;
         //   mat2.SetShader(&shader);
         //   Texture texture2("C:\\Users\\jiasheng.huang\\Documents\\GitHub\\MySoftRender\\SoftRender\\Assets\\Lisa0.tga");
          //  mat2.SetTexture(&texture2);

            Model model("C:\\Users\\jiasheng.huang\\Documents\\GitHub\\MySoftRender\\SoftRender\\Assets\\tong.obj");
            model.SetMaterial(0, mat);
        //    model.SetMaterial(1, mat2);
        //    model.SetMaterial(2, mat);
            
            
          //  setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(5, 5, -5)));
           
      //   Object obj(Box, mat2);
            Mesh Plane = CreatePlane(glm::vec3(-2, 0, -1.5), glm::vec3(-2, 0, 1.5), glm::vec3(2, 0, 1.5), glm::vec3(2, 0, -1.5), glm::vec3(0, 1, 0));
         // Mesh Box2 = CreateBox(glm::vec3(-5, 0.0, 0.0), 1.5f);
         // Mesh Box3 = CreateBox(glm::vec3(10, 0.0, 0.0), 1.5f);
         // Mesh Box4 = CreateBox(glm::vec3(-10, 0.0, 0.0), 1.5f);
         // Mesh Box5 = CreateBox(glm::vec3(-15, 0.0, 0.0), 1.5f);
       //   Mesh Box1 = CreateBox(glm::vec3(5, 0.0, 0.0), 1.0f);
          //  Material mat2(glm::vec4(0.5, 0.5, 0.5, 0.5),glm::vec4(0,0,0,0),32);
            Object Ground(Plane, mat2);
           
         
		  //  V2F o1 = shader.VertexShader(V1);
			//V2F o2 = shader.VertexShader(V2);
			//V2F o3 = shader.VertexShader(V3);
         // shader.Viewplanes.resize(6, glm::vec4(0));
            
		   // o1.windowPos = ViewPortMatrix* o1.windowPos;
			//o2.windowPos = ViewPortMatrix* o2.windowPos;
		  //  o3.windowPos = ViewPortMatrix* o3.windowPos;
          float angle = 0.0f;
          int px = 0;
          int py = 0;
            //While application is running
          std::thread t(ShowFps, gWindow);
          t.detach();
         dirLight = new DirectionLight(glm::vec3(1, -1, 1), glm::vec3(1, 1, 1), glm::vec3(0.5f, 0.5f, 0.5f),0.25f);
         pointLight = new PointLight(glm::vec3(1, 1, 1), glm::vec3(1, 0, 0), glm::vec3(1, 1, 1), 0.25f, 1.0f, 0.09f, 0.032f);
          spotLight = new SpotLight(glm::vec3(0,3, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1), glm::vec3(0.5, 0.5, 0.5), 0.25f, 1.0f, 0.09f, 0.032f,13.0f,15.5f);
 
          dirLight->Color=glm::vec3(0,1,0);
         pointLight->Color = glm::vec3(1, 0, 0);

            while (!quit)
            {         
                
                //Handle events on queue
                while (SDL_PollEvent(&event) != 0)
                {
                    
                    if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym) {
                        case SDLK_w: {
                            camera.MoveForward(0.2f);
                            break;
                        }
                        case SDLK_s: {
                            camera.MoveForward(-0.2f);
                            break;
                        }
                        case SDLK_a: {
                            camera.MoveRight(-0.2f);
                            break;
                        }
                        case SDLK_d: {
                            camera.MoveRight(0.2f);
                            break;
                        }
                        case SDLK_q: {
                            camera.MoveUp(0.2f);
                            break;
                        }
                        case SDLK_e: {
                            camera.MoveUp(-0.2f);
                            break;
                        }
                        case SDLK_UP:{
                            camera.RotatePitch(2.0);
                            break;
                        }
                        case SDLK_DOWN: {
                            camera.RotatePitch(-2.0);
                            break;
                        }
                        case SDLK_LEFT: {
                            camera.RotateYaw(2.0);
                            break;
                        }
                        case SDLK_RIGHT: {
                            camera.RotateYaw(-2.0);
                            break;
                        }
                        }
             
                    }

                    else if (event.type == SDL_MOUSEBUTTONDOWN) {
                        if (SDL_BUTTON_LEFT == event.button.button) {
                            px = event.button.x;        
                            py = event.button.y;
                        }
                    }
                    else if (event.type == SDL_MOUSEMOTION) {
                        if (SDL_BUTTON_LEFT == event.button.button) {
                            int movex = event.button.x - px;
                            px = event.button.x;
                            int movey = event.button.y - py;
                            py = event.button.y;
                         
                            camera.RotateYaw(-movex*0.05f);
                            camera.RotatePitch(-movey * 0.05f);                     
                       }
                       
                    }
                

                    //User requests quit
                    else if (event.type == SDL_QUIT)
                    {
                        quit = true;
                        
                    }
                }
                setViewMatrix(camera.ViewMatrix());
                setProjectMatrix(camera.PerspectiveMatrix());
                camera.UpdateCameraVectors();
                //fps
                fps++;
              
              
                //Clear screen
                //std::cout << fps/(float)t1 << std::endl;


               SDL_SetRenderDrawColor(gRenderer, 75, 75, 75, 255);
                SDL_RenderClear(gRenderer);
                FrontBuffer->ClearColorBuffer(glm::vec4(75, 75, 75, 255));
                FrontBuffer->ClearDepthBuffer();
             //   ScanLineTriangle(o1, o2, o3);
				
                //Rendering
          //     setModelMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0, 0.0, 0.0)));
               
              
              shader.UpdateViewPlanes();
             
            //  DrawObject(obj);
           //   std::thread first(Draw, gRenderer, shader, FrontBuffer, SCREEN_WIDTH, SCREEN_HEIGHT);
            //  std::thread second(Draw2, gRenderer, shader, FrontBuffer, SCREEN_WIDTH, SCREEN_HEIGHT);
           //   first.join();
            //  second.join();   
             



              DrawModel(model);
             DrawObject(Ground);

#pragma omp parallel for num_threads(4) 
              for (int i = 0; i < SCREEN_HEIGHT; i++) {      
                  for (int k = 0; k < SCREEN_WIDTH; k++) {
                      int xy = 4 * (i * SCREEN_WIDTH + k);
                      char* p = FrontBuffer->colorBuffer.data();
                     if (*(p + xy) == 75) {
                         continue;
                      }
                      SDL_SetRenderDrawColor(gRenderer, *(p + xy), *(p + xy + 1), *(p + xy + 2), *(p + xy + 3));
                      SDLDrawPixel(k, i);

                  }
              }
       
              
              SwapBuffer();
              
            //    angle += 5.0f;
                //Update screen
                SDL_RenderPresent(gRenderer);
            }

          
        }
        
    }

    //Free resources and close SDL
    close();
   
    return 0;
}