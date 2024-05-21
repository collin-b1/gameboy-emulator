#pragma once

#include <vector>
#include <cstdint>

constexpr uint16_t VRAM_START = 0x8000;
constexpr uint16_t VRAM_END = 0x9FFF;

constexpr uint16_t OAM_START = 0xFE00;
constexpr uint16_t OAM_END = 0xFE9F;

class PPU
{
public:
    PPU();

    uint8_t read(uint16_t);
    void write(uint16_t, uint8_t);

private:
    // 0x8000 - 0x9FFF: VRAM
    std::vector<uint8_t> vram;

    // 0xFE00 - 0xFE9F: OAM
    std::vector<uint8_t> oam;

    // 0xFF40: LCDC - LCD Control
    uint8_t lcdc;

    // 0xFF41: STAT - LCD Status
    uint8_t stat;

    // 0xFF42 – 0xFF43: SCY, SCX: Background viewport Y position, X position
    uint8_t scy, scx;

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