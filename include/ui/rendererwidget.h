#pragma once

#include "core.h"
#include "definitions.h"
#include <QObject>
#include <QWidget>
#include <array>
#include <memory>

constexpr int RENDER_SCALE = 3;

class RendererWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RendererWidget(QWidget *parent = nullptr);

public slots:
    void update_frame(const std::array<u32, SCREEN_SIZE> &frame_buffer);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::array<u32, SCREEN_SIZE> frame_buffer;
    QImage frame_image;
};