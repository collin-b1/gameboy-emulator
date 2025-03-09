#include "gobby.h"
#include "cart.h"
#include "cpu.h"
#include "ppu.h"

#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

Gobby::Gobby() : 
    cart{},
    interrupts{},
    timer{ interrupts },
    ppu{ interrupts },
    mmu{ cart, ppu, interrupts, timer },
    cpu{ mmu, interrupts }
{}

Gobby::Gobby(std::string rom) :
    cart{},
    interrupts{},
    timer{ interrupts },
    ppu{ interrupts },
    mmu{ cart, ppu, interrupts, timer },
    cpu{ mmu, interrupts }
{
    load_game(rom, "");
    cpu.init_post_boot();
    mmu.bus_write(0xFF50, 0x01);
}

Gobby::Gobby(std::string rom, std::string boot_rom) :
    cart{},
    interrupts{},
    timer{ interrupts },
    ppu{ interrupts },
    mmu{ cart, ppu, interrupts, timer },
    cpu{ mmu, interrupts }
{
    load_game(rom, boot_rom);
}

bool Gobby::load_game(std::string rom_name, std::string boot_rom)
{
    std::string rom_path{ ROMS_DIRECTORY + rom_name };
    std::string boot_rom_path{ ROMS_DIRECTORY + boot_rom };

    bool rom_loaded = cart.load_rom(rom_path);
    bool boot_rom_loaded = cart.load_boot_rom(boot_rom_path);

    if (!rom_loaded)
    {
        return false;
    }

    cart.print_headers();

    return true;
}

void Gobby::tick_systems()
{
    // CPU tick
    auto cpu_cycles = cpu.next_instruction();

    // PPU tick
    for (auto i{ 0 }; i < cpu_cycles * 4; ++i)
    {
        ppu.tick();
    }

    // Timer tick
    for (auto i{ 0 }; i < cpu_cycles * 4; ++i)
    {
        timer.tick();
    }

    // Serial tick
    // TODO: Implement serial
    /*
    for (auto i{ 0 }; i < cpu_cycles; ++i)
    {
        serial.tick();
    }
    */

    // Joypad tick
    // TODO: Implement joypad
}

int main(int argc, char* argv[])
{
    std::string rom_name{"02-interrupts.gb"};

    if (argc > 1)
    {
        rom_name = argv[1];
    }

    Gobby gobby(rom_name);

    bool cpu_running = true;
    auto frame_duration = std::chrono::duration<double, std::milli>(16.74); // Each frame is ~16.74ms
    while (cpu_running)
    {
        auto frame_start = std::chrono::high_resolution_clock::now();
        
        for (int i{ 0 }; i < 70224; ++i) // 70224 dots per frame
        {
            gobby.tick_systems();
        }

        auto frame_end = std::chrono::high_resolution_clock::now();
        auto elapsed = frame_end - frame_start;

        if (elapsed < frame_duration)
        {
            std::this_thread::sleep_for(frame_duration);
        }
    }

    return 0;
}