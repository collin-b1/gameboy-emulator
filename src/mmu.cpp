#include "mmu.h"
#include "ppu.h"
#include <iomanip>
#include <iostream>

MMU::MMU(Cart &cart, PPU &ppu, InterruptManager &imu, Timer &timer, Joypad &joypad)
    : cart(cart), ppu(ppu), imu(imu), wram(), io(), hram(), timer(timer), serial(), joypad(joypad), svbk(0)
{
}

u8 MMU::bus_read(u16 addr) const
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

    // Interrupt flag
    else if (addr == 0xFF0F)
    {
        // std::cout << "[IMU] Write 0x" << std::hex << addr << ": 0x" << static_cast<int>(data) << std::endl;
        return imu.read(addr);
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
    else
    {
        std::cerr << "\nMMU read address not implemented! At: 0x" << std::hex << addr << std::endl;
    }

    return 0;
}

void MMU::bus_write(u16 addr, u8 data)
{
    // Cartridge ROM
    if (addr >= 0x0000 && addr <= 0x7FFF)
    {
        cart.write(addr, data);
    }

    // Video RAM (VRAM)
    else if (addr >= 0x8000 && addr <= 0x9FFF)
    {
        //        if (addr >= 0x8000 && addr < 0x8020)
        //        {
        //            std::cout << "Write to VRAM " << std::hex << addr << ": " << +data << "\n";
        //        }
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
        // std::cout << "Boot ROM disabled!" << std::endl;

        std::cout << "Tile data at 0x8000:\n";
        for (int i = 0x8000; i < 0x8100; i += 16)
        {
            std::cout << "Tile " << ((i - 0x8000) / 16) << ": ";
            for (int j = 0; j < 16; ++j)
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)bus_read_word(i + j) << ' ';
            std::cout << '\n';
        }

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
        // std::cout << "[IMU] Write 0x" << std::hex << addr << ": 0x" << static_cast<int>(data) << std::endl;
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
        // std::cout << "[IMU] Write 0x" << std::hex << addr << ": 0x" << static_cast<int>(data) << std::endl;
        imu.write(addr, data);
    }

    else
    {
        std::cerr << "\nMMU write address not implemented! At: 0x" << std::hex << addr << std::endl;
    }
}

u16 MMU::bus_read_word(u16 addr) const
{
    return (0xFF & bus_read(addr) | (bus_read(addr + 1) << 8));
}

void MMU::bus_write_word(u16 addr, u16 data)
{
    u8 lsb = data & 0xFF;
    bus_write(addr, lsb);

    u8 msb = (data >> 8) & 0xFF;
    bus_write(addr + 1, msb);
}

bool MMU::is_boot_rom_disabled() const
{
    return cart.is_boot_rom_disabled();
}

bool MMU::load_rom(std::string rom_path)
{
    return cart.load_rom(rom_path);
}
void MMU::stop_div(bool stopped)
{
    timer.stop_div(stopped);
}
