#pragma once

#include <cstdint>
#include <string>

#include "cpu.h"
#include "mmu.h"
#include "ppu.h"
#include "interrupt.h"

#include <memory>

class Gobby
{
public:
    Gobby();
    Gobby(std::string);
    Gobby(std::string, std::string);
    void tick_systems();
    bool load_game(std::string rom, std::string boot_rom);
private:
    Cart cart;
    CPU cpu;
    MMU mmu;
    PPU ppu;
    Timer timer;
    InterruptManager interrupts;
};