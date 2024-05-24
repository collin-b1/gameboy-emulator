#include "ppu.h"
#include <iostream>

enum PPU_Mode : uint8_t
{
    MODE_HBLANK = 0,
    MODE_VBLANK = 1,
    MODE_OAM = 2,
    MODE_VRAM = 3
};

PPU::PPU(InterruptManager& imu) : 
    imu(imu),
    mode(MODE_HBLANK),
    vram{},
    oam{},
    lcdc(0),
    scy(0), scx(0), 
    ly(0), lyc(0),
    bgp(0), 
    obp0(0), obp1(0), 
    wy(0), wx(0), key1(0), 
    vbk(0), 
    hdma1(0), hdma2(0), hdma3(0), 
    hdma4(0), hdma5(0), 
    bcps(0), bcpd(0), 
    ocps(0), ocpd(0)
{}

uint8_t PPU::read(uint16_t addr)
{
    switch (addr)
    {
    case 0xFF40: return lcdc;
    case 0xFF41:
    {
        return 0x00
            | mode
            | (ly == lyc) << 2;
    }
    case 0xFF42: return scy;
    case 0xFF43: return scx;
    case 0xFF44: return 0x90; // ly
    case 0xFF45: return lyc;
    case 0xFF47: return bgp;
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
    case 0xFF68: return bcps;
    case 0xFF69: return bcpd;
    case 0xFF6A: return ocps;
    case 0xFF6B: return ocpd;
    default:
    {
        if (addr >= VRAM_START && addr <= VRAM_END)
        {
            return vram[addr - VRAM_START];
        }
        else if (addr >= OAM_START && addr <= OAM_END)
        {
            return oam[addr - OAM_START];
        }
        return 0;
    }
    }
}

void PPU::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0xFF40: lcdc = data; break;
    case 0xFF41: /*stat = data;*/ break;
    case 0xFF42: scy = data; break;
    case 0xFF43: scx = data; break;
    case 0xFF44: ly = data; break;
    case 0xFF45: lyc = data; break;
    case 0xFF47: bgp = data; break;
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
    case 0xFF68: bcps = data; break;
    case 0xFF69: bcpd = data; break;
    case 0xFF6A: ocps = data; break;
    case 0xFF6B: ocpd = data; break;
    default: 
    {
        if (addr >= VRAM_START && addr <= VRAM_END)
        {
            vram[addr - VRAM_START] = data;
        }
        else if (addr >= OAM_START && addr <= OAM_END)
        {
            oam[addr - OAM_START] = data;
        }
    }
    }
}
