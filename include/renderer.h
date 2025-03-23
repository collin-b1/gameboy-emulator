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
    void render_frame_buffer() const;
    void push_pixel(uint8_t x, uint8_t y, uint8_t color_id);
    static uint32_t rgb_to_u32(uint8_t, uint8_t, uint8_t);
private:
    std::array<uint32_t, SCREEN_WIDTH * SCREEN_HEIGHT> frame_buffer;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> frame_texture;

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
};