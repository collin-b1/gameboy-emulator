#pragma once

#include <array>
#include <cstdint>

#include "interrupt.h"
#include "memory.h"
#include "renderer.h"

constexpr uint16_t VRAM_START = 0x8000;
constexpr uint16_t VRAM_END = 0x9FFF;

constexpr uint16_t OAM_START = 0xFE00;
constexpr uint16_t OAM_END = 0xFE9F;

constexpr uint32_t DOTS_PER_CYCLE = 4;
constexpr uint32_t CYCLES_PER_SCANLINE = 114 * DOTS_PER_CYCLE;
constexpr uint32_t CYCLES_PER_FRAME = 154 * CYCLES_PER_SCANLINE;

constexpr uint32_t CYCLES_OAM = 80;
constexpr uint32_t CYCLES_HBLANK = 80;

struct Object
{
    uint8_t y;
    uint8_t x;
    uint8_t tile_index;
    union {
        uint8_t flags;
        struct
        {
            uint8_t priority : 1;
            uint8_t y_flip : 1;
            uint8_t x_flip : 1;
            uint8_t dmg_palette : 1;
            uint8_t bank : 1;
            uint8_t cgb_palette : 3;
        };
    } flags;
};

class MMU;

class PPU final : public IMemory
{
public:
    PPU(InterruptManager &imu, Renderer &renderer);

    void bind_mmu(MMU *_mmu);

    [[nodiscard]] uint8_t read(uint16_t) const override;
    void write(uint16_t, uint8_t) override;

    // Viewport operations
    // Calculates the bottom coordinate of the viewport
    [[nodiscard]] uint8_t get_viewport_y() const;
    // Calculates the right coordinate of the viewport
    [[nodiscard]] uint8_t get_viewport_x() const;

    // Tile operations
    [[nodiscard]] Object get_object_from_oam(uint8_t);
    [[nodiscard]] const uint8_t *get_tile_data(uint8_t) const;

    void tick(uint16_t);
    void dma_transfer(uint8_t);
    void render_scanline();
    void draw_frame();

private:
    MMU *mmu;
    InterruptManager &imu;
    Renderer &renderer;

    uint32_t mode_clock;

    // 0x8000 - 0x9FFF: VRAM
    std::array<uint8_t, VRAM_END - VRAM_START + 1> vram;

    // 0xFE00 - 0xFE9F: OAM
    std::array<uint8_t, OAM_END - OAM_START + 1> oam;

    // std::array<uint8_t, SCREEN_WIDTH * SCREEN_HEIGHT> screen;

    // 0xFF40: LCDC - LCD Control
    // Can be modified at any time during the frame, even mid-scanline
    union {
        uint8_t lcdc;
        struct
        {
            // Non-CGB: BG and window display
            // CGB: BG and Window master priority
            uint8_t bg_window_enable_priority : 1;
            uint8_t obj_enable : 1;
            uint8_t obj_size : 1;                 // 0: 8x8, 1: 8x16
            uint8_t bg_tile_map_area : 1;         // 0: 9800-9BFF, 1: 9C00-9FFF
            uint8_t bg_window_tile_data_area : 1; // 0: 8800-97FF, 1: 8000-8FFF
            uint8_t window_enable : 1;
            uint8_t tile_map_area : 1; // 0: 9800-9BFF, 1: 9C00-9FFF
            uint8_t lcd_ppu_enable : 1;
        };
    } lcdc;

    // 0xFF41: STAT - LCD Mode
    union {
        uint8_t stat;
        struct
        {
            uint8_t : 1;
            uint8_t lyc_int_select : 1;
            uint8_t mode_2_int_select : 1;
            uint8_t mode_1_int_select : 1;
            uint8_t mode_0_int_select : 1;
            uint8_t lyc_ly_comparison : 1;
            uint8_t ppu_mode : 2;
        };
    } stat;

    // 0xFF42: SCY: Background viewport Y position
    uint8_t scy;

    // 0xFF43: SCX: Background viewport X position
    uint8_t scx;

    // 0xFF44: LY - LCDC Y-Coordinate (Current Scanline)
    uint8_t ly;

    // 0xFF45: LYC - LY Compare
    uint8_t lyc;

    // 0xFF46: DMA: OAM DMA src & start
    uint8_t dma;

    // 0xFF47: BGP - Background Palette Data (non-CGB mode only)
    // 0: White, 1: Light gray, 2: Dark gray, 3: Black
    union {
        uint8_t bgp;
        struct
        {
            uint8_t id3 : 2;
            uint8_t id2 : 2;
            uint8_t id1 : 2;
            uint8_t id0 : 2;
        };
    } bgp;

    // 0xFF48 - 0xFF49: OBP0, OBP1 - Object Palette Data (non-CGB mode only)
    uint8_t obp0, obp1;

    // 0xFF4A � 0xFF4B: WY, WX: Window Y position and (X position + 7)
    // Window will be visible if wx in [0, 166] and wy in [0, 143]
    uint8_t wy, wx;

    // 0xFF4D: KEY1 - Prepare Speed Switch (CGB only)
    uint8_t key1;

    // 0xFF4F: VBK - VRAM Bank (CGB only)
    uint8_t vbk;

    // 0xFF51 - 0xFF55: HDMA1 - HDMA5 - DMA Transfer (CGB only)
    uint8_t hdma1, hdma2, hdma3, hdma4, hdma5;

    // 0xFF68: BCPS/BGPI - Background Palette Index (CGB only)
    // Can never be accessed outside VBlank and HBlank
    union {
        uint8_t bcps;
        struct
        {
            uint8_t auto_increment : 1;
            uint8_t : 1;
            uint8_t address : 6;
        };
    } bcps;

    // 0xFF69: BCPD/BGPD - Background Palette Data (CGB only)
    union {
        uint16_t bcpd;
        struct
        {
            uint16_t red_intensity : 5;
            uint16_t green_intensity : 5;
            uint16_t blue_intensity : 5;
            uint16_t : 1;
        };
    } bcpd;

    // 0xFF6A - 0xFF6B: OCPS/OBPI - Object Palette Index (CGB only)
    uint8_t ocps, ocpd;
};