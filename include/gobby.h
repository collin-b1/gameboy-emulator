#pragma once

#include <cstdint>
#include <string>

#include "cpu.h"
#include "mmu.h"
#include "ppu.h"
#include "interrupt.h"

#include <memory>
#include <QMainWindow>

class Gobby
{
public:
    Gobby(QMainWindow*);
    void tick_systems();
    bool load_game(const std::string& rom, const std::string& boot_rom);
private:
    uint64_t clock;

    Cart cart;
    CPU cpu;
    MMU mmu;
    PPU ppu;
    Renderer renderer;
    Timer timer;
    InterruptManager interrupts;
};