#include "mmu.h"
#include <cstdint>
#include <iostream>

MMU::MMU(Cart& cart, PPU& ppu, InterruptManager& imu) :
    cart(cart),
    ppu(ppu),
    imu(imu),
    wram{},
    hram{},
    timer(Timer{}),
    serial(Serial{}),
    joypad(Joypad{}),
    svbk(0)
{}

uint8_t MMU::bus_read(uint16_t addr)
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

    else if (addr >= 0xFF04 && addr <= 0xFF07)
    {
        return timer.read(addr);
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

uint16_t MMU::bus_read_word(uint16_t addr)
{
    return (0xFF & bus_read(addr) | (bus_read(addr + 1) << 8));
}

void MMU::bus_write(uint16_t addr, uint8_t data)
{
    /*if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        std::cout << "Write " << std::hex << (int)data << std::endl;
        std::cout << std::endl;
    }*/
    //std::cout << "Writing value 0x" << std::hex << (int)data << " to address: 0x" << std::hex << (int)addr << std::dec << std::endl;
    
    // ROM Bank 0-n
    if (addr >= 0x0000 && addr <= 0x7FFF)
    {
        cart.write(addr, data);
    }

    // Video RAM (VRAM)
    else if (addr >= 0x8000 && addr <= 0x9FFF)
    {
        ppu.write(addr, data);
    }

    // Working RAM (WRAM)
    else if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        wram[addr - 0xC000] = data;
    }

    // Object Attribute Memory (OAM)
    else if (addr >= 0xFE00 && addr <= 0xFE9F)
    {
        ppu.write(addr, data);
    }

    // I/O Registers
    else if (addr >= 0xFF00 && addr <= 0xFF7F)
    {
        ppu.write(addr, data);
        imu.write(addr, data);
    }

    // High RAM
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