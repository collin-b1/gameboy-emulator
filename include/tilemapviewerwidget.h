#pragma once

#include "definitions.h"

#include "ppu.h"
#include "register.h"
#include <QImage>
#include <QWidget>

constexpr int DEBUG_SCREEN_WIDTH = 128;
constexpr int DEBUG_SCREEN_HEIGHT = 192;
constexpr int DEBUG_RENDER_SCALE = 2;

class TileMapViewerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TileMapViewerWidget(QWidget *parent = nullptr);

public slots:
    void update_tile_map(const std::array<u32, TILE_MAP_SIZE> &frame_buffer);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    std::array<u32, TILE_MAP_SIZE> tile_map_buffer;
    QPoint hovered_tile = {-1, -1};
    QImage tile_map_image;
};