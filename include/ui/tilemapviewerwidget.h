#pragma once

#include "cpu/register.h"
#include "definitions.h"
#include "ppu/ppu.h"
#include "ui/framelistener.h"
#include <QImage>
#include <QObject>
#include <QWidget>

class TileMapViewerWidget : public QWidget, public IFrameListener
{
    Q_OBJECT
public:
    explicit TileMapViewerWidget(QWidget *parent = nullptr);
    void on_frame_ready(const std::span<const u32> frame_buffer, size_t size) override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    std::array<u32, TILE_MAP_SIZE> tile_map_buffer;
    QPoint hovered_tile = {-1, -1};
    QImage tile_map_image;
};