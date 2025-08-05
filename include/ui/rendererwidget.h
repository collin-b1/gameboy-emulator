#pragma once

#include "core.h"
#include "definitions.h"
#include <QObject>
#include <QWidget>
#include <array>
#include <memory>

constexpr int RENDER_SCALE = 3;

class RendererWidget : public QWidget, public IFrameListener
{
    Q_OBJECT

public:
    explicit RendererWidget(QWidget *parent = nullptr);
    void on_frame_ready(const std::span<const u32> frame_buffer, size_t size) override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::array<u32, SCREEN_SIZE> frame_buffer;
    QImage frame_image;
};