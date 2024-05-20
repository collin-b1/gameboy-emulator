#include <cstdint>
#include <bus.h>
#include <iostream>
// 0000	3FFF	16 KiB ROM bank 00				From cartridge, usually a fixed bank
// 4000	7FFF	16 KiB ROM Bank 01–NN			From cartridge, switchable bank via mapper(if any)
// 8000	9FFF	8 KiB Video RAM(VRAM)			In CGB mode, switchable bank 0 / 1
// A000	BFFF	8 KiB External RAM				From cartridge, switchable bank if any
// C000	CFFF	4 KiB Work RAM(WRAM)
// D000	DFFF	4 KiB Work RAM(WRAM)			In CGB mode, switchable bank 1–7
// E000	FDFF	Echo RAM(mirror of C000–DDFF)	Nintendo says use of this area is prohibited.
// FE00	FE9F	Object attribute memory(OAM)
// FEA0	FEFF	Not Usable						Nintendo says use of this area is prohibited.
// FF00	FF7F	I / O Registers
// FF80	FFFE	High RAM(HRAM)
// FFFF	FFFF	Interrupt Enable register (IE)

MemoryBus::MemoryBus() : cart{Cart()} 
{
    io = std::vector<uint8_t>(0xFF7F - 0xFF00, 0xFF);
    wram = std::vector<uint8_t>(0xE000 - 0xC000, 0xFF);
    hram = std::vector<uint8_t>(0xFFFE - 0xFF80, 0xFF);
}

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

    if (addr >= 0xFF00 && addr <= 0xFF7F)
    {
        return io[addr - 0xFF00];
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

    if (addr >= 0xFF00 && addr <= 0xFF7F)
    {
        io[addr - 0xFF00] = data;
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