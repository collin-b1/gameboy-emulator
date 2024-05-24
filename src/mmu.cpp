#include "mmu.h"
#include <cstdint>
#include <iostream>

MMU::MMU(Cart& cart, PPU& ppu, InterruptManager& imu) :
    cart(cart),
    ppu(ppu),
    imu(imu),
    wram(WRAM_END - WRAM_START + 1),
    hram(HRAM_END - HRAM_START + 1),
    timer(),
    serial(),
    joypad(),
    svbk(0)
{}

uint8_t MMU::bus_read(uint16_t addr) const
{
    // Cartridge ROM
    if (addr >= 0x0000 && addr <= 0x7FFF)
    {
        return cart.read(addr);
    }

    // Video RAM (VRAM)
    else if (addr >= 0x8000 && addr <= 0x9FFF)
    {
        return ppu.read(addr);
    }

    // RAM Bank
    else if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        return cart.read(addr);
    }

    // Working RAM (WRAM)
    else if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        return wram[addr - 0xC000];
    }

    // Echo RAM (shouldn't be used)
    else if (addr >= 0xE000 && addr <= 0xFDFF)
    {
        return wram[addr - 0xE000];
    }

    // Object Attribute Memory (OAM)
    else if (addr >= 0xFE00 && addr <= 0xFE9F)
    {
        return ppu.read(addr);
    }

    // Unusable Memory
    else if (addr >= 0xFEA0 && addr <= 0xFEFF)
    {
        return 0;
    }

    // I/O Registers
    else if (addr == 0xFF00)
    {
        return joypad.read(addr);
    }

    // Serial
    else if (addr >= 0xFF01 && addr <= 0xFF02)
    {
        return serial.read(addr);
    }

    // Timer
    else if (addr >= 0xFF04 && addr <= 0xFF07)
    {
        return timer.read(addr);
    }

    // Audio
    else if (addr >= 0xFF10 && addr <= 0xFF26)
    {
        // TODO: Implement Audio
        return 0;
    }

    // Wave pattern
    else if (addr >= 0xFF30 && addr <= 0xFF3F)
    {
        // TODO: Implement Audio
        return 0;
    }

    // LCD Control, Status, Pos, Scrolling, Palettes
    else if (addr >= 0xFF40 && addr <= 0xFF4B)
    {
        return ppu.read(addr);
    }

    // VRAM Bank Select
    else if (addr == 0xFF4F)
    {
        return ppu.read(addr);
    }

    // Bootrom toggle (non-zero is disabled)
    else if (addr == 0xFF50)
    {
        return cart.read(addr);
    }

    // VRAM DMA
    else if (addr >= 0xFF51 && addr <= 0xFF55)
    {
        return ppu.read(addr);
    }

    // BG / OBJ Palletes
    else if (addr >= 0xFF68 && addr <= 0xFF6B)
    {
        return ppu.read(addr);
    }

    // WRAM Bank Select (SVBK)
    else if (addr == 0xFF70)
    {
        return svbk;
    }

    // Interrupt Flag (IF)
    else if (addr == 0xFF0F)
    {
        return imu.read(addr);
    }

    // High RAM (HRAM)
    else if (addr >= 0xFF80 && addr <= 0xFFFE)
    {
        return hram[addr - 0xFF80];
    }

    // Interrupt Enable (IE)
    else if (addr == 0xFFFF)
    {
        return imu.read(addr);
    }

    return 0;
}

void MMU::bus_write(uint16_t addr, uint8_t data)
{
    // Cartridge ROM
    if (addr >= 0x0000 && addr <= 0x7FFF)
    {
        cart.write(addr, data);
    }

    // Video RAM (VRAM)
    else if (addr >= 0x8000 && addr <= 0x9FFF)
    {
        ppu.write(addr, data);
    }

    // RAM Bank
    else if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        cart.write(addr, data);
    }

    // Working RAM (WRAM)
    else if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        wram[addr - 0xC000] = data;
    }

    // Echo RAM (shouldn't be used)
    else if (addr >= 0xE000 && addr <= 0xFDFF)
    {
        wram[addr - 0xE000] = data;
    }

    // Object Attribute Memory (OAM)
    else if (addr >= 0xFE00 && addr <= 0xFE9F)
    {
        ppu.write(addr, data);
    }

    // Unusable Memory
    else if (addr >= 0xFEA0 && addr <= 0xFEFF)
    {
        // Do nothing
    }

    // I/O Registers
    // Joypad
    else if (addr == 0xFF00)
    {
        joypad.write(addr, data);
    }

    // Serial
    else if (addr >= 0xFF01 && addr <= 0xFF02)
    {
        serial.write(addr, data);
    }

    // Timer
    else if (addr >= 0xFF04 && addr <= 0xFF07)
    {
        timer.write(addr, data);
    }

    // Audio
    else if (addr >= 0xFF10 && addr <= 0xFF26)
    {
        // TODO: Implement Audio
    }

    // Wave pattern
    else if (addr >= 0xFF30 && addr <= 0xFF3F)
    {
        // TODO: Implement Audio
    }

    // LCD Control, Status, Pos, Scrolling, Palettes
    else if (addr >= 0xFF40 && addr <= 0xFF4B)
    {
        ppu.write(addr, data);
    }

    // VRAM Bank Select
    else if (addr == 0xFF4F)
    {
        ppu.write(addr, data);
    }

    // Bootrom toggle (non-zero is disabled)
    else if (addr == 0xFF50)
    {
        std::cout << "Boot ROM disabled!" << std::endl;
        cart.write(addr, data);
    }

    // VRAM DMA
    else if (addr >= 0xFF51 && addr <= 0xFF55)
    {
        ppu.write(addr, data);
    }

    // BG / OBJ Palletes
    else if (addr >= 0xFF68 && addr <= 0xFF6B)
    {
        ppu.write(addr, data);
    }

    // WRAM Bank Select (SVBK)
    else if (addr == 0xFF70)
    {
        svbk = data;
    }

    // Interrupt Flag (IF)
    else if (addr == 0xFF0F)
    {
        imu.write(addr, data);
    }

    // High RAM (HRAM)
    else if (addr >= 0xFF80 && addr <= 0xFFFE)
    {
        hram[addr - 0xFF80] = data;
    }

    // Interrupt Enable (IE)
    else if (addr == 0xFFFF)
    {
        imu.write(addr, data);
    }
}

uint16_t MMU::bus_read_word(uint16_t addr) const
{
    return (0xFF & bus_read(addr) | (bus_read(addr + 1) << 8));
}

void MMU::bus_write_word(uint16_t addr, uint16_t data)
{
    uint8_t lsb = data & 0xFF;
    bus_write(addr, lsb);

    uint8_t msb = (data >> 8) & 0xFF;
    bus_write(addr + 1, msb);
}

bool MMU::load_rom(std::string rom_path)
{
    return cart.load_rom(rom_path);
}