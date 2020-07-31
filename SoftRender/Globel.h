#pragma once
#ifndef _GLOBEL_H_
#define _GLOBEL_H
#include<vector>
#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include<thread>
#include <SDL.h>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include<omp.h>
#include"glm\glm\glm.hpp"
#include"glm\glm\gtc\matrix_transform.hpp"

extern class Material;

extern Material *currentMat;

extern class DirectionLight;

extern DirectionLight *dirLight;

extern class PointLight;

extern PointLight *pointLight;

extern class SpotLight;

extern SpotLight* spotLight;

extern class Shader;

extern Shader shader;

extern class Camera;

extern Camera camera;

const glm::vec3 Ambient = glm::vec3(0.3, 0.3, 0.3);


extern SDL_Renderer* gRenderer;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern class FrameBuffer;

extern FrameBuffer* FrontBuffer ;

extern FrameBuffer *BackBuffer;
#endif