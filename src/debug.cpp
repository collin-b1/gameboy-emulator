#include "debug.h"
#include <QPainter>

DebugWindow::DebugWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Tile Map");
    setFixedSize(DEBUG_SCREEN_WIDTH * RENDER_SCALE, DEBUG_SCREEN_HEIGHT * RENDER_SCALE);
}

void DebugWindow::bind_ppu(PPU *_ppu)
{
    this->ppu = _ppu;
}

QImage DebugWindow::render_tile(const u8 *tile_data)
{
    QImage tile(8, 8, QImage::Format_RGBA8888);

    for (auto y{0}; y < 8; ++y)
    {
        uint8_t byte1 = tile_data[y * 2];
        uint8_t byte2 = tile_data[y * 2 + 1];

        for (auto x{0}; x < 8; ++x)
        {
            uint8_t bit = 7 - x;
            uint8_t lo = (byte1 >> bit) & 1;
            uint8_t hi = (byte2 >> bit) & 1;
            uint8_t color = (hi << 1) | lo;
            tile.setPixel(x, y, pixel_colors[color]);
        }
    }
    return tile;
}

void DebugWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.scale(RENDER_SCALE, RENDER_SCALE);

    for (int tileIndex = 0; tileIndex < 256; ++tileIndex)
    {
        const u8 *tileData = ppu->get_tile_data(tileIndex);
        QImage tile = render_tile(tileData);

        // painter.drawImage(...);
    }
}