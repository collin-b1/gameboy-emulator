#pragma once

#include <vector>
#include <cstdint>

#include "interrupt.h"
#include "renderer.h"
#include "memory.h"

constexpr uint16_t VRAM_START = 0x8000;
constexpr uint16_t VRAM_END = 0x9FFF;

constexpr uint16_t OAM_START = 0xFE00;
constexpr uint16_t OAM_END = 0xFE9F;

enum PPU_Mode;

class PPU : public IMemory
{
public:
    PPU(InterruptManager& imu);

    uint8_t read(uint16_t) const override;
    void write(uint16_t, uint8_t) override;
    void tick();
    void scanline();

private:
    InterruptManager& imu;

    Renderer renderer;

    // 0x8000 - 0x9FFF: VRAM
    std::vector<uint8_t> vram;

    // 0xFE00 - 0xFE9F: OAM
    std::vector<uint8_t> oam;

    uint8_t screen[160 * 144];

    // 0xFF40: LCDC - LCD Control
    union
    {
        uint8_t lcdc;
        struct
        {
            bool lcd_ppu_enable;
            bool tile_map_area; // 0: 9800-9BFF, 1: 9C00-9FFF
            bool window_enable;
            bool bg_window_tile_data_area; // 0: 8800-97FF, 1: 8000-8FFF
            bool bg_tile_map_area; // 0: 9800-9BFF, 1: 9C00-9FFF
            bool obj_size; // 0: 8x8, 1: 8x16
            bool obj_enable;
            // Non-CGB: BG and window display
            // CGB: BG and Window master priority
            bool bg_window_enable_priority;
        };
    } lcdc;


    // 0xFF41: STAT - LCD Mode
    uint8_t mode;

    // 0xFF42 – 0xFF43: SCY, SCX: Background viewport Y position, X position
    uint8_t scy, scx;

    // 0xFF44: LY - LCDC Y-Coordinate
    uint8_t ly;

    // 0xFF45: LYC - LY Compare
    uint8_t lyc;

    // 0xFF47: BGP - Background Palette Data (non-CGB mode only)
    uint8_t bgp;

    // 0xFF48 - 0xFF49: OBP0, OBP1 - Object Palette Data (non-CGB mode only)
    uint8_t obp0, obp1;

    // 0xFF4A – 0xFF4B: WY, WX: Window Y position, X position + 7
    uint8_t wy, wx;

    // 0xFF4D: KEY1 - Prepare Speed Switch (CGB only)
    uint8_t key1;

    // 0xFF4F: VBK - VRAM Bank (CGB only)
    uint8_t vbk;

    // 0xFF51 - 0xFF55: HDMA1 - HDMA5 - DMA Transfer (CGB only)
    uint8_t hdma1, hdma2, hdma3, hdma4, hdma5;

    // 0xFF68: BCPS/BGPI - Background Palette Index (CGB only)
    uint8_t bcps;

    // 0xFF69: BCPD/BGPD - Background Palette Data (CGB only)
    uint8_t bcpd;

    // 0xFF6A - 0xFF6B: OCPS/OBPI - Object Palette Index (CGB only)
    uint8_t ocps, ocpd;
};