#include "renderer.h"
#include <SDL.h>

//constexpr std::array<uint32_t, 4> pixel_colors{
//    0xffffffff,    // 0b00
//    0xffffffff,      // 0b01
//    0xffffffff,     // 0b10
//    0x000000ff,     // 0b11
//};

Renderer::Renderer()
    : window(nullptr, &SDL_DestroyWindow)
    , renderer(nullptr, &SDL_DestroyRenderer)
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create window and renderer
    window.reset(SDL_CreateWindow("Gameboy Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        160, 144, 0));
    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
}

Renderer::~Renderer()
{
    SDL_Quit();
}

void Renderer::push_pixel()
{

}