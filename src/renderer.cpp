#include "renderer.h"
#include <SDL.h>

constexpr std::array<uint32_t, 4> pixel_colors{
    255,  // 0b00
    (85 << 24) | (85 << 16) | (85 << 8) | 255,  // 0b01
    (170 << 24) | (170 << 16) | (170 << 8) | 255,  // 0b10
    (255 << 24) | (255 << 16) | (255 << 8) | 255  // 0b11
};

Renderer::Renderer()
    : window(nullptr, &SDL_DestroyWindow)
    , renderer(nullptr, &SDL_DestroyRenderer)
    , frame_texture(nullptr, &SDL_DestroyTexture)
    , frame_buffer()
    , pixel_ptr(0)
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create window and renderer
    window.reset(SDL_CreateWindow("Gameboy Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        160, 144, 0));
    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

    frame_texture.reset(SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_BGRA8888,
        SDL_TEXTUREACCESS_STREAMING, 160, 144));
}

Renderer::~Renderer()
{
    SDL_Quit();
}

void Renderer::render_frame_buffer()
{
    // Clear the renderer
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);

    // Update frame
    SDL_LockTexture(frame_texture.get(), nullptr, (void**)frame_buffer.data(), nullptr);

    SDL_UnlockTexture(frame_texture.get());

    SDL_RenderCopy(renderer.get(), frame_texture.get(), nullptr, nullptr);
}

void Renderer::push_pixel(uint8_t color_id)
{
    frame_buffer[pixel_ptr] = pixel_colors[color_id & 0x3];
    pixel_ptr = (pixel_ptr + 1) % frame_buffer.size();
}
