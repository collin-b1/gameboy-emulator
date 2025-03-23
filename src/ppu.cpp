#include "ppu.h"

#include <iostream>

enum PPUMode : uint8_t
{
    MODE_HBLANK = 0,
    MODE_VBLANK = 1,
    MODE_OAM_SEARCH = 2,
    MODE_VRAM = 3
};

PPU::PPU(InterruptManager& imu)
    : imu(imu)
    , dot_accumulator(0)
    , vram()
    , oam()
    , lcdc(0)
    , stat(0)
    , scy(0), scx(0)
    , ly(0), lyc(0)
    , dma(0)
    , bgp(0)
    , obp0(0), obp1(0)
    , wy(0), wx(0), key1(0)
    , vbk(0)
    , hdma1(0), hdma2(0), hdma3(0)
    , hdma4(0), hdma5(0)
    , bcps(0), bcpd(0)
    , ocps(0), ocpd(0)
{}

uint8_t PPU::read(uint16_t addr) const
{
    switch (addr)
    {
    case 0xFF40: return lcdc.lcdc;
    case 0xFF41: return (stat.stat & ~0x6) | (ly == lyc) << 2;
    case 0xFF42: return scy;
    case 0xFF43: return scx;
    case 0xFF44: return ly;
    case 0xFF45: return lyc;
    case 0xFF46: return dma;
    case 0xFF47: return bgp.bgp;
    case 0xFF48: return obp0;
    case 0xFF49: return obp1;
    case 0xFF4A: return wy;
    case 0xFF4B: return wx;
    case 0xFF4D: return key1;
    case 0xFF4F: return vbk;
    case 0xFF51: return hdma1;
    case 0xFF52: return hdma2;
    case 0xFF53: return hdma3;
    case 0xFF54: return hdma4;
    case 0xFF55: return hdma5;
    case 0xFF68: return bcps.bcps;
    case 0xFF69: return bcpd.bcpd;
    case 0xFF6A: return ocps;
    case 0xFF6B: return ocpd;
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
            // OAM is accisible during Modes 0-1
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

void PPU::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0xFF40: lcdc.lcdc = data; break;
    case 0xFF41: stat.stat = (data & 0xF8) | (stat.stat & 0x7); break;
    case 0xFF42: scy = data; break;
    case 0xFF43: scx = data; break;
    case 0xFF44: /*ly = data*/; break; // Read Only
    case 0xFF45: lyc = data; break;
    case 0xFF46:
    {
        dma = data;
        // start DMA transfer from ROM or RAM to OAM
        // written value is transfer src addr divided by 0x100
        // TODO: implement DMA transfer
        break;
    }
    case 0xFF47: bgp.bgp = data; break;
    case 0xFF48: obp0 = data; break;
    case 0xFF49: obp1 = data; break;
    case 0xFF4A: wy = data; break;
    case 0xFF4B: wx = data; break;
    case 0xFF4D: key1 = data; break;
    case 0xFF4F: vbk = data; break;
    case 0xFF51: hdma1 = data; break;
    case 0xFF52: hdma2 = data; break;
    case 0xFF53: hdma3 = data; break;
    case 0xFF54: hdma4 = data; break;
    case 0xFF55: hdma5 = data; break;
    case 0xFF68: bcps.bcps = data; break;
    case 0xFF69: bcpd.bcpd = data; break;
    case 0xFF6A: ocps = data; break;
    case 0xFF6B: ocpd = data; break;
    default: 
    {
        if (addr >= VRAM_START && addr <= VRAM_END)
        {
            // VRAM is only accessible during Modes 0-2
            if (stat.ppu_mode != MODE_VRAM)
            {
                vram[addr - VRAM_START] = data;
            }
        }
        else if (addr >= OAM_START && addr <= OAM_END)
        {
            // OAM is accisible during Modes 0-1
            if (stat.ppu_mode != MODE_OAM_SEARCH && stat.ppu_mode != MODE_VRAM)
            {
                oam[addr - OAM_START] = data;
            }
        }
    }
    }
}

uint8_t PPU::get_viewport_y() const
{
    return (scy + 143) % 256;
}

uint8_t PPU::get_viewport_x() const
{
    return (scx + 159) % 256;
}

void PPU::tick(const uint16_t cycles)
{
    dot_accumulator += cycles * DOTS_PER_CYCLE;

    if (dot_accumulator >= DOTS_PER_CYCLE)
    {
      render_scanline();
      dot_accumulator -= DOTS_PER_CYCLE;
    }
}

const uint8_t* PPU::get_tile_data(uint8_t idx) const
{
    // LCDC.4 == 1, 0x8000-0x8FFF
    if (lcdc.bg_window_tile_data_area)
    {
        // 0x8000 is base pointer, Block 0 is 0-127, Block 1 is 128-255
        return vram.data() + idx * 16;
    }

    // LCDC.4 == 0
    // Block 2 (0x9000-0x97FF)
    if (idx < 128)
    {
        return vram.data() + 0x1000 + idx * 16;
    }
    // Block 1 (0x8800-0x8FFF)
    else
    {
        return vram.data() + 0x800 + (idx - 128) * 16;
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

Object PPU::get_object_from_oam(uint8_t idx)
{
    const auto obj = reinterpret_cast<Object*>(oam.data() + idx * 4);

    return *obj;
}

void PPU::render_scanline()
{
    if (!lcdc.lcd_ppu_enable) return;

    if (ly < SCREEN_HEIGHT) {
        // OAM search
        auto objs_found{ 0 };
        Object objs[10];
        for (auto obj_i{ 0 }; obj_i < 40; ++obj_i)
        {
            const auto obj = get_object_from_oam(obj_i);
            if (ly >= obj.y && ly <= obj.y + 8)
            {
                objs[objs_found] = obj;
            }
        }

        // Push pixel to frame buffer
        for (auto x{ 0 }; x < SCREEN_WIDTH; ++x)
        {
            // TODO: Get Tilemap index from VRAM. Pull BG tile data and display correct line
            const auto pixel_x = (x + scx) % SCREEN_WIDTH;
            const auto pixel_y = (ly + scy) % SCREEN_HEIGHT;

            const auto tile_x = pixel_x / 8;
            const auto tile_y = pixel_y / 8;

            const auto tile_map = lcdc.bg_tile_map_area ? 0x1C00 : 0x1800;

            const auto tile_map_index = vram[tile_map + (32 * tile_y + tile_x)];

            const auto tile_data = get_tile_data(tile_map_index);

            // std::cout << tile_data[0] << std::endl;

            const auto translated_x = pixel_x % 8;
            const auto translated_y = pixel_y % 8;

            const auto tile_upper = *(tile_data + 2 * translated_y);
            const auto tile_lower = *(tile_data + 2 * translated_y + 1);

            const auto tile_pixel = ((tile_upper >> (7 - translated_x)) & 0x1) | (((tile_lower >> (7 - translated_x)) & 0x1) << 1);

            const auto pixel_color = (bgp.bgp >> tile_pixel) & 0x3;

            renderer.push_pixel(pixel_x, pixel_y, pixel_color);
        }
    }

    // Increment LY
    ly = (ly + 1) % 154;
}

void PPU::draw_frame() const {
    renderer.render_frame_buffer();
}
