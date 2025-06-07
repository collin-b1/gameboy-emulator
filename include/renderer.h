#pragma once

#include <QWidget>
#include <array>
#include <memory>

constexpr uint8_t SCREEN_WIDTH = 160;
constexpr uint8_t SCREEN_HEIGHT = 144;
constexpr uint8_t RENDER_SCALE = 3;

class Renderer : public QWidget
{
    Q_OBJECT
public:
    explicit Renderer(QWidget *parent = nullptr);
    //~Renderer() = default;
    void render_frame_buffer();
    void push_pixel(uint8_t x, uint8_t y, uint8_t color_id);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::array<uint32_t, SCREEN_WIDTH * SCREEN_HEIGHT> frame_buffer;
    //    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> frame_texture;
    QImage frame_image;

    //    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    //    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;

    static constexpr std::array<uint32_t, 4> pixel_colors{0xFF000000, 0xFF555555, 0xFFAAAAAA, 0xFFFFFFFF};
};