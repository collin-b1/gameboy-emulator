#pragma once

#include "cpu/cpu.h"
#include "cpu/interrupt.h"
#include "definitions.h"
#include "joypad.h"
#include "memory/bus.h"
#include "ppu/ppu.h"
#include <string>

class GameboyCore
{
public:
    explicit GameboyCore();
    bool load_rom(const std::string &path);
    void tick_systems();
    void reset();
    void init_post_boot();

    void set_button_pressed(Gobby::JoypadInput button, bool pressed);

    PPU &get_ppu();

private:
    u64 clock;
    bool paused;

    Cart cart;
    CPU cpu;
    PPU ppu;
    Bus bus;
    Joypad joypad;
    Timer timer;
    InterruptManager interrupts;
};