#include "renderer.h"

#include <iostream>
#include <SDL.h>

constexpr uint8_t render_scale = 3;

constexpr std::array<uint32_t, 4> pixel_colors{
    0xFF,
    static_cast<uint32_t>(0x55 << 24) | static_cast<uint32_t>(0x55 << 16) | static_cast<uint32_t>(0x55 << 8) | 0xFF,
    static_cast<uint32_t>(0xAA << 24) | static_cast<uint32_t>(0xAA << 16) | static_cast<uint32_t>(0xAA << 8) | 0xFF,
    static_cast<uint32_t>(0xFF << 24) | static_cast<uint32_t>(0xFF << 16) | static_cast<uint32_t>(0xFF << 8) | 0xFF
};

Renderer::Renderer()
    : window(nullptr, &SDL_DestroyWindow)
    , renderer(nullptr, &SDL_DestroyRenderer)
    , frame_texture(nullptr, &SDL_DestroyTexture)
    , frame_buffer()
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create window and renderer
    window.reset(SDL_CreateWindow("Gameboy Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        160 * render_scale, 144 * render_scale, 0));
    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

    SDL_RenderSetScale(renderer.get(), render_scale, render_scale);

    frame_texture.reset(SDL_CreateTexture(renderer.get(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT));

    // for (auto i{ 0 }; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
    //     frame_buffer[i] = 0xFFFFFFFF;
    // }

    frame_buffer.fill(0xFFFFFFFF);
}

Renderer::~Renderer()
{
    SDL_Quit();
}

void Renderer::render_frame_buffer() const {
    // std::cout << "Rendering frame buffer" << frame_buffer.at(0) << std::endl;
    // Clear the renderer
    // SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);

    // Update frame
    // void* pixels = frame_buffer.data();
    // int pitch;
    // SDL_LockTexture(frame_texture.get(), nullptr, &pixels, &pitch);

    //std::copy(pixels, framebuffer.data())

    // SDL_UnlockTexture(frame_texture.get());

    constexpr int pitch = SCREEN_WIDTH * sizeof(uint32_t);
    //SDL_LockTexture(frame_texture.get(), nullptr, reinterpret_cast<void**>(&frame_buffer), &pitch);
    SDL_UpdateTexture(frame_texture.get(), nullptr, frame_buffer.data(), pitch);
    //SDL_UnlockTexture(frame_texture.get());

    SDL_RenderClear(renderer.get());
    SDL_RenderCopy(renderer.get(), frame_texture.get(), nullptr, nullptr);
    SDL_RenderPresent(renderer.get());
}

void Renderer::push_pixel(const uint8_t x, const uint8_t y, const uint8_t color_id)
{
    frame_buffer[y * SCREEN_WIDTH + x] = pixel_colors[color_id & 0x3];
}

static uint32_t rgb_to_u32(const uint8_t r, const uint8_t g, const uint8_t b) {
    return r << 24 | g << 16 | b << 8 | 0xFF;
}