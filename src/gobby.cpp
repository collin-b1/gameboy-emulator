#include "gobby.h"
#include "cart.h"
#include "cpu.h"
#include "ppu.h"

#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

constexpr uint32_t cycles_per_frame = 70224;
constexpr uint32_t cycles_per_tick = cycles_per_frame / 60;

Gobby::Gobby()
    : clock(0)
    , cart{}
    , interrupts{}
    , timer{ interrupts }
    , ppu{ interrupts }
    , mmu{ cart, ppu, interrupts, timer }
    , cpu{ mmu, interrupts }
{}

Gobby::Gobby(std::string rom)
    : clock(0)
    , cart{}
    , interrupts{}
    , timer{ interrupts }
    , ppu{ interrupts }
    , mmu{ cart, ppu, interrupts, timer }
    , cpu{ mmu, interrupts }
{
    load_game(rom, "");
    cpu.init_post_boot();
    mmu.bus_write(0xFF50, 0x01);
}

Gobby::Gobby(std::string rom, std::string boot_rom)
    : clock(0)
    , cart{}
    , interrupts{}
    , timer{ interrupts }
    , ppu{ interrupts }
    , mmu{ cart, ppu, interrupts, timer }
    , cpu{ mmu, interrupts }
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
    uint32_t cycles{ 0 };
    while (cycles < cycles_per_tick)
    {
        uint8_t cpu_cycles = cpu.next_instruction();
        cycles += cpu_cycles;

        for (uint8_t i{ 0 }; i < cpu_cycles * 4; ++i)
        {
            ppu.tick();
        }

        timer.tick(cpu_cycles);
        //std::cout << cycles << " " << cpu.get_state() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    std::string rom_name{"02-interrupts.gb"};

    if (argc > 1)
    {
        rom_name = argv[1];
    }

    auto gobby = std::make_unique<Gobby>(rom_name);

    bool cpu_running = true;
    auto frame_duration = std::chrono::duration<double, std::milli>(16.74); // Each frame is ~16.74ms

    while (cpu_running)
    {
        auto frame_start = std::chrono::high_resolution_clock::now();
        
        for (int i{ 0 }; i < 70224; ++i) // 70224 dots per frame
        {
            gobby->tick_systems();
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