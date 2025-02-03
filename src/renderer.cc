#include "renderer.h"

void RenderTriangle(SDL_Renderer* renderer, float x1, float y1, 
                                          float x2, float y2, 
                                          float x3, float y3, float r, float g, float b){

    std::vector< SDL_Vertex > verts;

    SDL_Vertex v1 = { SDL_FPoint{ x1, y1 }, SDL_FColor{ r, g, b }};
    SDL_Vertex v2 = { SDL_FPoint{ x2, y2 }, SDL_FColor{ r, g, b }};
    SDL_Vertex v3 = { SDL_FPoint{ x3, y3 }, SDL_FColor{ r, g, b }};

    verts.push_back(v1);
    verts.push_back(v2);
    verts.push_back(v3);

    SDL_RenderGeometry( renderer, nullptr, verts.data(), verts.size(), nullptr, 0 );

    /*
    SDL_SetRenderDrawColor(renderer,0 , 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderLine( renderer, x1, y1, x2, y2);
    SDL_RenderLine( renderer, x2, y2, x3, y3);
    SDL_RenderLine( renderer, x3, y3, x1, y1);*/
}