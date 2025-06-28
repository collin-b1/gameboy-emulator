#include "debug.h"
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

DebugWindow::DebugWindow(QWidget *parent) : QWidget(parent)
{
    setFixedSize(DEBUG_SCREEN_WIDTH * DEBUG_RENDER_SCALE, DEBUG_SCREEN_HEIGHT * DEBUG_RENDER_SCALE);
    setMouseTracking(true);
}

void DebugWindow::set_tile_map(const QImage &image)
{
    this->tile_map_image = image;
    update();
}

void DebugWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (!tile_map_image.isNull())
    {
        // Render Tile Map
        painter.scale(DEBUG_RENDER_SCALE, DEBUG_RENDER_SCALE);
        painter.drawImage(0, 0, tile_map_image);

        // Render Mouse Helper
        if (hovered_tile.x() >= 0 && hovered_tile.y() >= 0)
        {
            QRect tile_rect(hovered_tile.x() * 8, hovered_tile.y() * 8, 8, 8);
            painter.setPen(Qt::red);
            painter.drawRect(tile_rect);
        }
    }
}
void DebugWindow::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint pos = event->pos();
    const auto pos_x = pos.x() / (8 * DEBUG_RENDER_SCALE);
    const auto pos_y = pos.y() / (8 * DEBUG_RENDER_SCALE);

    const auto hovered_tile_idx = pos_y * DEBUG_TILES_PER_ROW + pos_x;

    if (hovered_tile_idx >= 0 && hovered_tile_idx < DEBUG_TILES_PER_ROW * DEBUG_TOTAL_ROWS)
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
