#pragma once

#include "definitions.h"

#include "ppu.h"
#include "register.h"
#include <QImage>
#include <QWidget>

constexpr int DEBUG_SCREEN_WIDTH = 128;
constexpr int DEBUG_SCREEN_HEIGHT = 192;
constexpr int DEBUG_RENDER_SCALE = 2;

class DebugWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DebugWindow(QWidget *parent = nullptr);
    void set_tile_map(const QImage &image);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint hovered_tile = {-1, -1};
    QImage tile_map_image;
};