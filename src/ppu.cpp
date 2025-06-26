#include "ppu.h"
#include "mmu.h"

#include <iostream>

enum PPUMode : u8
{
    MODE_HBLANK = 0,
    MODE_VBLANK = 1,
    MODE_OAM_SEARCH = 2,
    MODE_VRAM = 3
};

PPU::PPU(InterruptManager &imu, Renderer &renderer)
    : imu(imu)
    , mmu(nullptr)
    , renderer(renderer)
    , mode_clock(0)
    , window_line_y(0)
    , visible_sprites()
    , visible_sprite_count(0)
    , vram()
    , oam()
    , lcdc(0)
    , stat(0)
    , scy(0)
    , scx(0)
    , ly(0)
    , lyc(0)
    , dma(0)
    , bgp(0)
    , obp0(0)
    , obp1(0)
    , wy(0)
    , wx(0)
    , key1(0)
    , vbk(0)
    , hdma1(0)
    , hdma2(0)
    , hdma3(0)
    , hdma4(0)
    , hdma5(0)
    , bcps(0)
    , bcpd(0)
    , ocps(0)
    , ocpd(0)
{
    stat.ppu_mode = MODE_OAM_SEARCH;
}

void PPU::bind_mmu(MMU *_mmu)
{
    this->mmu = _mmu;
}

u8 PPU::read(u16 addr) const
{
    switch (addr)
    {
    case 0xFF40:
        return lcdc.lcdc;
    case 0xFF41:
        return (stat.stat & ~0x6) | (ly == lyc) << 2;
    case 0xFF42:
        return scy;
    case 0xFF43:
        return scx;
    case 0xFF44:
        return ly;
    case 0xFF45:
        return lyc;
    case 0xFF46:
        return dma;
    case 0xFF47:
        return bgp.bgp;
    case 0xFF48:
        return obp0;
    case 0xFF49:
        return obp1;
    case 0xFF4A:
        return wy;
    case 0xFF4B:
        return wx;
    case 0xFF4D:
        return key1;
    case 0xFF4F:
        return vbk;
    case 0xFF51:
        return hdma1;
    case 0xFF52:
        return hdma2;
    case 0xFF53:
        return hdma3;
    case 0xFF54:
        return hdma4;
    case 0xFF55:
        return hdma5;
    case 0xFF68:
        return bcps.bcps;
    case 0xFF69:
        return bcpd.bcpd;
    case 0xFF6A:
        return ocps;
    case 0xFF6B:
        return ocpd;
    default:
        if (addr >= VRAM_START && addr <= VRAM_END)
        {
            // VRAM is only accessible during Modes 0-2
            if (stat.ppu_mode != MODE_VRAM)
            {
                return vram[addr - VRAM_START];
            }
            else
            {
                return 0xFF;
            }
        }
        else if (addr >= OAM_START && addr <= OAM_END)
        {
            // OAM is accessible during Modes 0-1
            if (stat.ppu_mode != MODE_OAM_SEARCH && stat.ppu_mode != MODE_VRAM)
            {
                return oam[addr - OAM_START];
            }
            else
            {
                return 0xFF;
            }
        }
        else
        {
            return 0;
        }
    }
}

void PPU::write(u16 addr, u8 data)
{
    switch (addr)
    {
    case 0xFF40:
        lcdc.lcdc = data;
        break;
    case 0xFF41:
        stat.stat = (data & 0xF8) | (stat.stat & 0x7);
        break;
    case 0xFF42:
        scy = data;
        break;
    case 0xFF43:
        scx = data;
        break;
    case 0xFF44:
        break; // Read Only
    case 0xFF45:
        lyc = data;
        break;
    case 0xFF46: {
        dma = data;
        // start DMA transfer from ROM or RAM to OAM
        // written value is transfer src addr divided by 0x100
        dma_transfer(data);
        break;
    }
    case 0xFF47:
        bgp.bgp = data;
        break;
    case 0xFF48:
        obp0 = data;
        break;
    case 0xFF49:
        obp1 = data;
        break;
    case 0xFF4A:
        wy = data;
        break;
    case 0xFF4B:
        wx = data;
        break;
    case 0xFF4D:
        key1 = data;
        break;
    case 0xFF4F:
        vbk = data;
        break;
    case 0xFF51:
        hdma1 = data;
        break;
    case 0xFF52:
        hdma2 = data;
        break;
    case 0xFF53:
        hdma3 = data;
        break;
    case 0xFF54:
        hdma4 = data;
        break;
    case 0xFF55:
        hdma5 = data;
        break;
    case 0xFF68:
        bcps.bcps = data;
        break;
    case 0xFF69:
        bcpd.bcpd = data;
        break;
    case 0xFF6A:
        ocps = data;
        break;
    case 0xFF6B:
        ocpd = data;
        break;
    default: {
        if (addr >= VRAM_START && addr <= VRAM_END)
        {
            // VRAM is only accessible during Modes 0-2
            if (stat.ppu_mode != MODE_HBLANK && stat.ppu_mode != MODE_OAM_SEARCH)
            {
                return;
            }
            vram[addr - VRAM_START] = data;
        }
        else if (addr >= OAM_START && addr <= OAM_END)
        {
            // OAM is accessible during Modes 0-1
            if (stat.ppu_mode == MODE_VRAM || stat.ppu_mode == MODE_OAM_SEARCH)
            {
                return;
            }
            oam[addr - OAM_START] = data;
        }
    }
    }
}

u8 PPU::get_viewport_y() const
{
    return (scy + 143) % 256;
}

u8 PPU::get_viewport_x() const
{
    return (scx + 159) % 256;
}

void PPU::tick(const u16 cycles)
{
    if (!lcdc.lcd_ppu_enable)
    {
        mode_clock = 0;
        ly = 0;
        stat.ppu_mode = MODE_HBLANK;
        return;
    }

    mode_clock += cycles;

    switch (stat.ppu_mode)
    {
    case MODE_OAM_SEARCH:
        if (mode_clock >= 80)
        {
            // OAM search
            visible_sprite_count = 0;

            const auto sprite_height = lcdc.obj_size ? (u8)16 : (u8)8;

            for (auto obj_i{0}; obj_i < 40 && visible_sprite_count < 10; ++obj_i)
            {
                const auto obj = get_object_from_oam(obj_i);
                const auto obj_y = obj.y - 16;

                if (ly >= obj_y && ly < (obj_y + sprite_height))
                {
                    visible_sprites[visible_sprite_count++] = obj;
                }
            }

            mode_clock %= 80;
            stat.ppu_mode = MODE_VRAM;
        }
        break;

    case MODE_VRAM:
        if (mode_clock >= 172)
        {
            mode_clock %= 172;
            render_scanline();
            stat.ppu_mode = MODE_HBLANK;
        }
        break;

    case MODE_HBLANK:
        if (mode_clock >= 204)
        {
            mode_clock %= 204;
            ++ly;

            if (lcdc.window_enable && ly >= wy && ly < SCREEN_HEIGHT && wx < 166)
            {
                ++window_line_y;
            }

            if (ly == 144)
            {
                stat.ppu_mode = MODE_VBLANK;
                imu.request_interrupt(InterruptSource::INTERRUPT_VBLANK);
            }
            else
            {
                stat.ppu_mode = MODE_OAM_SEARCH;
            }
        }
        break;

    case MODE_VBLANK:
        if (mode_clock >= 456)
        {
            mode_clock %= 456;
            ++ly;

            if (ly == 154)
            {
                draw_frame();
                ly = 0;
                window_line_y = 0;
                stat.ppu_mode = MODE_OAM_SEARCH;
            }
        }
        break;
    }
}

void PPU::dma_transfer(u8 source_byte)
{
    u16 base = source_byte << 8;
    for (int i = 0; i < 0xA0; ++i)
        oam[i] = mmu->bus_read(base + i);
}

const u8 *PPU::get_tile_data(u8 idx) const
{
    if (lcdc.bg_window_tile_data_area)
    {
        // 0x8000 is base pointer, index is unsigned
        return &vram[idx * 16];
    }
    else
    {
        // 0x9000 is base pointer, index is signed
        int8_t signed_idx = static_cast<int8_t>(idx);
        const u16 base_addr = 0x9000 + signed_idx * 16;
        return &vram[base_addr - VRAM_START];
    }
}

// void PPU::next_dot()
// {
//     if (!lcdc.lcd_ppu_enable) return;
//
//     renderer.render_frame_buffer();
//
//     switch (stat.ppu_mode)
//     {
//     // Mode 2: OAM Scan
//     // Duration: 80 dots
//     case MODE_OAM_SEARCH:
//         // Search for OBJs which overlap this scanline
//
//         break;
//
//     // Mode 3: Drawing Pixels
//     // Duration: 172 to 289 dots
//     case MODE_VRAM:
//         // renderer.push_pixel(scx, scy, );
//         break;
//
//     // Mode 0: Horizontal Blank
//     // Duration: 376 - Mode 3's duration
//     case MODE_HBLANK:
//         break;
//
//     // Mode 1: Vertical Blank
//     // Duration: 4560 dots
//     case MODE_VBLANK:
//
//         break;
//
//     // Unreachable
//     default:
//         exit(8);
//     }
// }

Object PPU::get_object_from_oam(u8 idx)
{
    const auto obj = reinterpret_cast<Object *>(oam.data() + idx * 4);

    return *obj;
}

void PPU::render_scanline()
{
    if (ly >= SCREEN_HEIGHT)
        return;

    // Push pixel to frame buffer
    for (u8 x{0}; x < SCREEN_WIDTH; ++x)
    {
        const auto use_window = lcdc.window_enable && ly >= wy && x >= (wx - 7);
        const auto bg_enabled = lcdc.bg_window_enable_priority;

        u8 tile_x, tile_y, translated_x, translated_y;
        u16 tile_map_addr;
        u8 final_pixel_color;

        if (bg_enabled)
        {
            if (use_window) // Window
            {
                u8 window_x = x - (wx - 7);
                u8 window_y = ly - wy;

                tile_x = window_x / 8;
                tile_y = window_line_y / 8;

                translated_x = window_x % 8;
                translated_y = window_line_y % 8;

                tile_map_addr = (lcdc.window_tile_map_area ? 0x9C00 : 0x9800) + (32 * tile_y + tile_x);
            }
            else // Background
            {
                const auto pixel_x = (x + scx) % 256;
                const auto pixel_y = (ly + scy) % 256;

                tile_x = pixel_x / 8;
                tile_y = pixel_y / 8;

                translated_x = pixel_x % 8;
                translated_y = pixel_y % 8;

                tile_map_addr = (lcdc.bg_tile_map_area ? 0x9C00 : 0x9800) + (32 * tile_y + tile_x);
            }

            const auto tile_map_index = vram[tile_map_addr - VRAM_START];
            const auto color_id = get_color_id_from_tile(tile_map_index, translated_x, translated_y);

            const auto bg_pixel_color = (bgp.bgp >> (color_id * 2)) & 0x3;
            final_pixel_color = bg_pixel_color;
        }
        else
        {
            final_pixel_color = 0;
        }

        // Sprites
        for (auto sprite_idx{0}; sprite_idx < visible_sprite_count; ++sprite_idx)
        {
            const auto sprite = visible_sprites[sprite_idx];
            u8 sprite_x = x - (sprite.x - 8);
            if (sprite_x < 0 || sprite_x >= 8)
                continue; // Sprite X does not overlap with current pixel

            u8 sprite_y = ly - (sprite.y - 16);
            u8 sprite_height = lcdc.obj_size ? 16 : 8;
            if (sprite_y < 0 || sprite_y >= sprite_height)
                continue; // Sprite Y does not overlap with current pixel

            if (sprite.flags.x_flip)
                sprite_x = 7 - sprite_x;

            if (sprite.flags.y_flip)
                sprite_y = sprite_height - 1 - sprite_y;

            // Change tile index if in 8x16 mode
            u8 tile_idx = sprite.tile_index;
            if (sprite_height == 16)
            {
                tile_idx &= 0xFE;
                if (sprite_y >= 8)
                {
                    tile_idx += 1;
                    sprite_y -= 8;
                }
            }

            const auto sprite_color_id = get_color_id_from_tile(tile_idx, sprite_x, sprite_y);

            if (sprite_color_id == 0)
                continue; // Transparent pixel

            u8 palette = sprite.flags.dmg_palette ? obp1 : obp0;
            u8 sprite_pixel_color = (palette >> (sprite_color_id * 2)) & 0x3;

            const auto bg_transparent = (final_pixel_color == 0);
            const auto sprite_priority = !(sprite.flags.priority);

            if (bg_transparent || sprite_priority || !bg_enabled)
            {
                final_pixel_color = sprite_pixel_color;
            }

            break;
        }

        renderer.push_pixel(x, ly, final_pixel_color);
    }
}

void PPU::draw_frame()
{
    renderer.render_frame_buffer();
}

u8 PPU::get_color_id_from_tile(u8 tile, u8 x, u8 y) const
{
    const auto tile_data = get_tile_data(tile);

    const auto tile_upper = *(tile_data + 2 * y);
    const auto tile_lower = *(tile_data + 2 * y + 1);

    const auto color_id = ((tile_upper >> (7 - x)) & 0x1) | (((tile_lower >> (7 - x)) & 0x1) << 1);

    return color_id;
}
