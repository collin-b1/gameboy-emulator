#pragma once

#include <cstdint>
#include <string>

#include "cpu.h"
#include "mmu.h"
#include "ppu.h"
#include "interrupt.h"
#include "renderer.h"

#include <memory>

class Gobby
{
public:
    Gobby();
    void tick_systems();
    bool load_rom(std::string path);
private:
    Cart cart;
    CPU cpu;
    MMU mmu;
    PPU ppu;
    InterruptManager interrupts;
    Renderer renderer;
};