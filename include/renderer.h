#pragma once

#include <SDL.h>
#include <memory>

class Renderer
{
public:
    Renderer();
    ~Renderer();
    void push_pixel();
private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
};