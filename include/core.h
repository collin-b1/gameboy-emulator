#pragma once

#include "cpu.h"
#include "definitions.h"
#include "interrupt.h"
#include "mmu.h"
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

    const PPU *get_ppu() const;
    u8 *get_tile_map_buffer() const;

private:
    u64 clock;

    Cart cart;
    CPU cpu;
    PPU ppu;
    MMU mmu;
    Timer timer;
    InterruptManager interrupts;
};