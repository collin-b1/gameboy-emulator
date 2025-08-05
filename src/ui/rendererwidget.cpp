#include "ui/rendererwidget.h"
#include <QObject>
#include <QPainter>
#include <iostream>
#include <memory>

RendererWidget::RendererWidget(QWidget *parent)
    : QWidget(parent)
    , frame_buffer{}
    , frame_image(reinterpret_cast<uchar *>(frame_buffer.data()), SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_RGBA8888)
{
    frame_buffer.fill(0);
    setMinimumSize(SCREEN_WIDTH * RENDER_SCALE, SCREEN_HEIGHT * RENDER_SCALE);
}

void RendererWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    // painter.scale(RENDER_SCALE, RENDER_SCALE);
    painter.drawImage(this->rect(), frame_image);
}

void RendererWidget::on_frame_ready(const std::span<const u32> ppu_frame_buffer, size_t size)
{
    if (size != SCREEN_SIZE)
    {
        return;
    }
    std::copy(ppu_frame_buffer.begin(), ppu_frame_buffer.end(), frame_buffer.begin());
    update();
}
