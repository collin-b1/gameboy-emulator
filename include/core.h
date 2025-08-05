#pragma once

#include "cpu/cpu.h"
#include "cpu/interrupt.h"
#include "definitions.h"
#include "joypad.h"
#include "memory/mmu.h"
#include "ppu.h"
#include <QObject>
#include <string>

class GameboyCore : public QObject
{
    Q_OBJECT
public:
    explicit GameboyCore(QObject *parent = nullptr);
    bool load_rom(const std::string &path);
    void tick_systems();
    void reset();
    void init_post_boot();

    void set_button_pressed(Gobby::JoypadInput button, bool pressed);

    PPU &get_ppu();
    // u8 *get_tile_map_buffer() const;

private:
    u64 clock;

    Cart cart;
    CPU cpu;
    PPU ppu;
    MMU mmu;
    Joypad joypad;
    Timer timer;
    InterruptManager interrupts;
};