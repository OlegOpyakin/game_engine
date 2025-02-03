#include <SDL3/SDL.h>
#include <vector>
#pragma once

void RenderTriangle(SDL_Renderer* renderer, float x1, float y1, 
                                          float x2, float y2, 
                                          float x3, float y3, float r, float g, float b);