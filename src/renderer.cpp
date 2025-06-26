#include "renderer.h"

#include <iostream>
// #include <SDL.h>
#include <QPainter>

Renderer::Renderer(QWidget *parent)
    : QWidget(parent)
    , frame_image(reinterpret_cast<uchar *>(frame_buffer.data()), SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_RGBA8888)
//    , window(nullptr, &SDL_DestroyWindow)
//    , renderer(nullptr, &SDL_DestroyRenderer)
//    , frame_texture(nullptr, &SDL_DestroyTexture)
//    , frame_buffer()
{
    // Initialize SDL
    //    SDL_Init(SDL_INIT_VIDEO);
    //
    //    // Create window and renderer
    //    window.reset(SDL_CreateWindow("Gameboy Emulator",
    //        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    //        160 * render_scale, 144 * render_scale, 0));
    //    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
    //
    //    SDL_RenderSetScale(renderer.get(), render_scale, render_scale);
    //
    //    frame_texture.reset(SDL_CreateTexture(renderer.get(),
    //        SDL_PIXELFORMAT_RGBA8888,
    //        SDL_TEXTUREACCESS_STREAMING,
    //        SCREEN_WIDTH, SCREEN_HEIGHT));

    // for (auto i{ 0 }; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
    //     frame_buffer[i] = 0xFFFFFFFF;
    // }

    frame_buffer.fill(pixel_colors.at(0));
    setFixedSize(SCREEN_WIDTH * RENDER_SCALE, SCREEN_HEIGHT * RENDER_SCALE);
}

// Renderer::~Renderer()
//{
//     SDL_Quit();
// }

void Renderer::render_frame_buffer()
{
    // std::cout << "Rendering frame buffer" << frame_buffer.at(0) << std::endl;
    // Clear the renderer
    // SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);

    // Update frame
    // void* pixels = frame_buffer.data();
    // int pitch;
    // SDL_LockTexture(frame_texture.get(), nullptr, &pixels, &pitch);

    // std::copy(pixels, framebuffer.data())

    // SDL_UnlockTexture(frame_texture.get());

    //    constexpr int pitch = SCREEN_WIDTH * sizeof(uint32_t);
    //    SDL_UpdateTexture(frame_texture.get(), nullptr, frame_buffer.data(), pitch);
    //
    //    SDL_RenderClear(renderer.get());
    //    SDL_RenderCopy(renderer.get(), frame_texture.get(), nullptr, nullptr);
    //    SDL_RenderPresent(renderer.get());
    update();
}

void Renderer::push_pixel(const u8 x, const u8 y, const u8 color_id)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
    frame_buffer[y * SCREEN_WIDTH + x] = pixel_colors[color_id & 0x3];
}

void Renderer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.scale(RENDER_SCALE, RENDER_SCALE);
    painter.drawImage(0, 0, frame_image);
}