#pragma once

#include "definitions.h"
#include "ppu.h"
#include "register.h"
#include <QImage>
#include <QObject>
#include <QWidget>

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
    void leaveEvent(QEvent *event) override;

private:
    std::array<u32, TILE_MAP_SIZE> tile_map_buffer;
    QPoint hovered_tile = {-1, -1};
    QImage tile_map_image;
};