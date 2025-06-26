#pragma once

#include "definitions.h"

#include "ppu.h"
#include "register.h"
#include <QImage>
#include <QWidget>

constexpr u8 DEBUG_SCREEN_WIDTH = 256;
constexpr u8 DEBUG_SCREEN_HEIGHT = 256;

class DebugWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DebugWindow(QWidget *parent = nullptr);
    QImage render_tile(const u8 *tile_data);
    void bind_ppu(PPU *ppu);

    // void updateRegisters(Registers &registers);
    // void updateMemory(std::array<uint8_t, 0x8000> &memory);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    PPU *ppu;
};