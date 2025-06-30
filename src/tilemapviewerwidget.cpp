#include "tilemapviewerwidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

TileMapViewerWidget::TileMapViewerWidget(QWidget *parent)
    : QWidget(parent)
    , tile_map_image(reinterpret_cast<uchar *>(tile_map_buffer.data()), TILE_MAP_WIDTH, TILE_MAP_HEIGHT,
                     QImage::Format_RGBA8888)

{
    tile_map_buffer.fill(0);

    setMinimumSize(TILE_MAP_WIDTH * DEBUG_RENDER_SCALE, TILE_MAP_HEIGHT * DEBUG_RENDER_SCALE);
    setMouseTracking(true);
}

void TileMapViewerWidget::update_tile_map(const std::array<u32, TILE_MAP_SIZE> &ppu_tile_map_buffer)
{
    std::copy(ppu_tile_map_buffer.begin(), ppu_tile_map_buffer.end(), tile_map_buffer.begin());
    update();
}

void TileMapViewerWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (!tile_map_image.isNull())
    {
        // Render Tile Map
        // painter.scale(DEBUG_RENDER_SCALE, DEBUG_RENDER_SCALE);
        painter.drawImage(this->rect(), tile_map_image);

        const auto tile_width = float(width()) / TILE_MAP_TILES_PER_ROW;
        const auto tile_height = float(height()) / TILE_MAP_TOTAL_ROWS;

        // Render Mouse Helper
        if (hovered_tile.x() >= 0 && hovered_tile.y() >= 0)
        {
            QRect tile_rect(hovered_tile.x() * tile_width, hovered_tile.y() * tile_height, tile_width, tile_height);
            painter.setPen(Qt::red);
            painter.drawRect(tile_rect);
        }
    }
}
void TileMapViewerWidget::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint pos = event->pos();

    const auto tile_width = float(width()) / TILE_MAP_TILES_PER_ROW;
    const auto tile_height = float(height()) / TILE_MAP_TOTAL_ROWS;

    const auto pos_x = int(pos.x() / tile_width);
    const auto pos_y = int(pos.y() / tile_height);

    const auto hovered_tile_idx = pos_y * TILE_MAP_TILES_PER_ROW + pos_x;

    if (hovered_tile_idx >= 0 && hovered_tile_idx < TILE_MAP_TILES_PER_ROW * TILE_MAP_TOTAL_ROWS)
    {
        hovered_tile = QPoint(pos_x, pos_y);
        update();

        const u16 tile_addr = 0x8000 + hovered_tile_idx * 16;
        const QString tile_tooltip =
            QString("Tile %1 (0x%2)").arg(hovered_tile_idx).arg(std::format("{:04X}", tile_addr));
        QToolTip::showText(event->globalPosition().toPoint(), tile_tooltip, this);
    }
    else
    {
        hovered_tile = QPoint(-1, -1);
        update();
    }
}
