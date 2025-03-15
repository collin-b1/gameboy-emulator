#pragma once

#include <SDL.h>
#include <memory>
#include <array>

constexpr uint8_t SCREEN_WIDTH = 160;
constexpr uint8_t SCREEN_HEIGHT = 144;

class Renderer
{
public:
    Renderer();
    ~Renderer();
    void render_frame_buffer();
    void push_pixel(uint8_t);
private:
    size_t pixel_ptr;

    std::array<uint32_t, SCREEN_WIDTH * SCREEN_HEIGHT> frame_buffer;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> frame_texture;

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
};