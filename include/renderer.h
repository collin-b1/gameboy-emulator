#pragma once

#include <SDL.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};