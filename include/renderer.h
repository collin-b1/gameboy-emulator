#pragma once

#include "definitions.h"

#include <QWidget>
#include <array>
#include <memory>

constexpr int SCREEN_WIDTH = 160;
constexpr int SCREEN_HEIGHT = 144;
constexpr int RENDER_SCALE = 3;

class Renderer : public QWidget
{
    Q_OBJECT
public:
    explicit Renderer(QWidget *parent = nullptr);
    //~Renderer() = default;
    void render_frame_buffer();
    void push_pixel(u8 x, u8 y, u8 color_id);
    void blank_line(u8 y);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::array<u32, SCREEN_WIDTH * SCREEN_HEIGHT> frame_buffer;
    //    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> frame_texture;
    QImage frame_image;

    //    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    //    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
};