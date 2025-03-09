#pragma once
#include <array>
#include <memory>

#include "cart.h"
#include "ppu.h"
#include "interrupt.h"
#include "timer.h"
#include "serial.h"
#include "joypad.h"

constexpr uint16_t WRAM_START = 0xC000;
constexpr uint16_t WRAM_END = 0xDFFF;

constexpr uint16_t HRAM_START = 0xFF80;
constexpr uint16_t HRAM_END = 0xFFFE;

class MMU
{
public:
    MMU(Cart&, PPU&, InterruptManager&, Timer&);
    uint8_t bus_read(uint16_t) const;
    uint16_t bus_read_word(uint16_t) const;
    void bus_write(uint16_t addr, uint8_t data);
    void bus_write_word(uint16_t addr, uint16_t data);
    bool load_rom(std::string path);
private:
    std::array<uint8_t, WRAM_END - WRAM_START + 1> wram;
    std::array<uint8_t, HRAM_END - HRAM_START + 1> hram;
    uint8_t svbk;

    Cart& cart;
    PPU& ppu;
    InterruptManager& imu;
    Timer& timer;
    Serial serial;
    Joypad joypad;
};