#include "bus.h"
#include <cstdint>
#include <iostream>

MemoryBus::MemoryBus() : 
    cart{ Cart()},
    ppu{ PPU() },
    imu{ InterruptManager() },
    wram(WRAM_END - WRAM_START + 1), 
    hram(HRAM_END - HRAM_START + 1),
    svbk(0)
{}

MemoryBus::~MemoryBus() {}

uint8_t MemoryBus::bus_read(uint16_t addr)
{
    if (addr >= 0x0000 && addr <= 0x7FFF)
    {
        return cart.read(addr);
    }

    if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        return wram[addr - 0xC000];
    }

    if (addr >= 0xFF80 && addr <= 0xFFFE)
    {
        return hram[addr - 0xFF80];
    }

    if (addr == 0xFF44)
    {
        return 0x90;
    }

    return 0;
}

uint16_t MemoryBus::bus_read_word(uint16_t addr)
{
    return (0xFF & bus_read(addr) | (bus_read(addr + 1) << 8));
}

void MemoryBus::bus_write(uint16_t addr, uint8_t data)
{
    /*if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        std::cout << "Write " << std::hex << (int)data << std::endl;
        std::cout << std::endl;
    }*/
    //std::cout << "Writing value 0x" << std::hex << (int)data << " to address: 0x" << std::hex << (int)addr << std::dec << std::endl;
    if (addr >= 0x0000 && addr <= 0x7FFF)
    {
        cart.write(addr, data);
    }

    if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        wram[addr - 0xC000] = data;
    }

    if (addr >= 0xFF80 && addr <= 0xFFFE)
    {
        hram[addr - 0xFF80] = data;
    }
}

void MemoryBus::bus_write_word(uint16_t addr, uint16_t data)
{
    uint8_t lsb = data & 0xFF;
    bus_write(addr, lsb);

    uint8_t msb = (data >> 8) & 0xFF;
    bus_write(addr + 1, msb);
}

Cart& MemoryBus::get_cart()
{
    return cart;
}