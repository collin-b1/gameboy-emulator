#pragma once

#include <QObject>
#include <array>
#include <cstdint>

#include "definitions.h"
#include "interrupt.h"
#include "memory.h"

constexpr u16 VRAM_START = 0x8000;
constexpr u16 VRAM_END = 0x9FFF;
constexpr u16 VRAM_SIZE = VRAM_END - VRAM_START + 1;

constexpr u16 OAM_START = 0xFE00;
constexpr u16 OAM_END = 0xFE9F;

constexpr u32 DOTS_PER_CYCLE = 4;
constexpr u32 CYCLES_PER_SCANLINE = 114 * DOTS_PER_CYCLE;
constexpr u32 CYCLES_PER_FRAME = 154 * CYCLES_PER_SCANLINE;

constexpr u32 CYCLES_OAM = 80;
constexpr u32 CYCLES_HBLANK = 80;

struct Object
{
    u8 y;
    u8 x;
    u8 tile_index;
    union {
        u8 flags;
        struct
        {
            u8 priority : 1;
            u8 y_flip : 1;
            u8 x_flip : 1;
            u8 dmg_palette : 1;
            u8 bank : 1;
            u8 cgb_palette : 3;
        };
    } flags;
};

enum PPUMode : u8
{
    MODE_HBLANK = 0,
    MODE_VBLANK = 1,
    MODE_OAM_SEARCH = 2,
    MODE_VRAM = 3
};

class MMU;

class PPU final : public QObject, public IMemory
{
    Q_OBJECT

public:
    explicit PPU(InterruptManager &imu, QObject *parent = nullptr);

    void bind_mmu(MMU *_mmu);

    [[nodiscard]] u8 read(u16) const override;
    void write(u16, u8) override;

    // Viewport operations
    // Calculates the bottom coordinate of the viewport
    [[nodiscard]] u8 get_viewport_y() const;
    // Calculates the right coordinate of the viewport
    [[nodiscard]] u8 get_viewport_x() const;

    // Tile operations
    [[nodiscard]] Object get_object_from_oam(u8);
    [[nodiscard]] const u8 *get_tile_data(u8) const;
    [[nodiscard]] u8 get_color_id_from_tile(u8 tile, u8 x, u8 y) const;
    void load_tile_map_buffer();

    // Frame buffer operations
    void push_pixel(u8 x, u8 y, u8 color_id);
    void blank_line(u8 y);

    void tick(u16);
    void increment_ly();
    void dma_transfer(u8);
    void render_scanline();
    void draw_frame();

signals:
    void frame_ready(const std::array<u32, SCREEN_SIZE> &frame_buffer);
    void tile_map_ready(const std::array<u32, TILE_MAP_SIZE> &tile_map_buffer);

private:
    std::array<u32, SCREEN_SIZE> frame_buffer;
    std::array<u32, TILE_MAP_SIZE> tile_map_buffer;

    MMU *mmu;
    InterruptManager &imu;

    PPUMode last_mode;
    u32 mode_clock;

    u8 window_line_y;

    // 0x8000 - 0x9FFF: VRAM
    std::array<u8, VRAM_END - VRAM_START + 1> vram;

    // 0xFE00 - 0xFE9F: OAM
    std::array<u8, OAM_END - OAM_START + 1> oam;

    // OAM Sprites
    std::array<Object, 10> visible_sprites;
    int visible_sprite_count;

    // std::array<u8, SCREEN_WIDTH * SCREEN_HEIGHT> screen;

    // 0xFF40: LCDC - LCD Control
    // Can be modified at any time during the frame, even mid-scanline
    union {
        u8 lcdc;
        struct
        {
            // Non-CGB: BG and window display
            // CGB: BG and Window master priority
            u8 bg_window_enable_priority : 1;
            u8 obj_enable : 1;
            u8 obj_size : 1;                 // 0: 8x8, 1: 8x16
            u8 bg_tile_map_area : 1;         // 0: 9800-9BFF, 1: 9C00-9FFF
            u8 bg_window_tile_data_area : 1; // 0: 8800-97FF, 1: 8000-8FFF
            u8 window_enable : 1;
            u8 window_tile_map_area : 1; // 0: 9800-9BFF, 1: 9C00-9FFF
            u8 lcd_ppu_enable : 1;
        };
    } lcdc;

    // 0xFF41: STAT - LCD Mode
    union {
        u8 stat;
        struct
        {
            u8 ppu_mode : 2;
            u8 lyc_ly_comparison : 1;
            u8 mode_0_int_select : 1;
            u8 mode_1_int_select : 1;
            u8 mode_2_int_select : 1;
            u8 lyc_int_select : 1;
            u8 : 1;
        };
    } stat;

    // 0xFF42: SCY: Background viewport Y position
    u8 scy;

    // 0xFF43: SCX: Background viewport X position
    u8 scx;

    // 0xFF44: LY - LCDC Y-Coordinate (Current Scanline)
    u8 ly;

    // 0xFF45: LYC - LY Compare
    u8 lyc;

    // 0xFF46: DMA: OAM DMA src & start
    u8 dma;

    // 0xFF47: BGP - Background Palette Data (non-CGB mode only)
    // 0: White, 1: Light gray, 2: Dark gray, 3: Black
    union {
        u8 bgp;
        struct
        {
            u8 id3 : 2;
            u8 id2 : 2;
            u8 id1 : 2;
            u8 id0 : 2;
        };
    } bgp;

    // 0xFF48 - 0xFF49: OBP0, OBP1 - Object Palette Data (non-CGB mode only)
    u8 obp0, obp1;

    // 0xFF4A - 0xFF4B: WY, WX: Window Y position and (X position + 7)
    // Window will be visible if wx in [0, 166] and wy in [0, 143]
    u8 wy, wx;

    // 0xFF4D: KEY1 - Prepare Speed Switch (CGB only)
    u8 key1;

    // 0xFF4F: VBK - VRAM Bank (CGB only)
    u8 vbk;

    // 0xFF51 - 0xFF55: HDMA1 - HDMA5 - DMA Transfer (CGB only)
    u8 hdma1, hdma2, hdma3, hdma4, hdma5;

    // 0xFF68: BCPS/BGPI - Background Palette Index (CGB only)
    // Can never be accessed outside VBlank and HBlank
    union {
        u8 bcps;
        struct
        {
            u8 auto_increment : 1;
            u8 : 1;
            u8 address : 6;
        };
    } bcps;

    // 0xFF69: BCPD/BGPD - Background Palette Data (CGB only)
    union {
        u16 bcpd;
        struct
        {
            u16 red_intensity : 5;
            u16 green_intensity : 5;
            u16 blue_intensity : 5;
            u16 : 1;
        };
    } bcpd;

    // 0xFF6A - 0xFF6B: OCPS/OBPI - Object Palette Index (CGB only)
    u8 ocps, ocpd;
};