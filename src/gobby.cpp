#include "gobby.h"
#include "cart.h"
#include "cpu.h"
#include "ppu.h"

#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

Gobby::Gobby()
    : clock(0)
    , cart{}
    , cpu{ mmu, interrupts }
    , mmu{ cart, ppu, interrupts, timer }
    , ppu{ interrupts }
    , timer{ interrupts }
    , interrupts{}
{}

Gobby::Gobby(const std::string &rom)
    : clock(0)
    , cart{}
    , interrupts{}
    , timer{ interrupts }
    , ppu{ interrupts }
    , mmu{ cart, ppu, interrupts, timer }
    , cpu{ mmu, interrupts }
{
    if (!load_game(rom, "")) {
        exit(1);
    }
    cpu.init_post_boot();
    mmu.bus_write(0xFF50, 0x01);
}

Gobby::Gobby(const std::string &rom, const std::string &boot_rom)
    : clock(0)
    , cart{}
    , interrupts{}
    , timer{ interrupts }
    , ppu{ interrupts }
    , mmu{ cart, ppu, interrupts, timer }
    , cpu{ mmu, interrupts }
{
    if (!load_game(rom, boot_rom)) {
        exit(1);
    }
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

    if (!boot_rom_loaded)
    {
        return false;
    }

    cart.print_headers();

    return true;
}

void Gobby::tick_systems()
{
    uint32_t cycles{ 0 };
    while (cycles < CYCLES_PER_FRAME)
    {
        const uint8_t cpu_cycles = cpu.next_instruction();
        cycles += cpu_cycles;

        // Timer tick
        timer.tick(cpu_cycles);

        // PPU tick
        ppu.tick(cpu_cycles);

        // std::cout << cycles << " " << cpu.get_state() << std::endl;
    }
    ppu.draw_frame();
}

int main(int argc, char *argv[])
{
    std::string rom_name{ "06-ld r,r.gb" };

    if (argc > 1)
    {
        rom_name = argv[1];
    }

    const auto gobby = std::make_unique<Gobby>(rom_name);

    bool cpu_running = true;
    const auto frame_duration = std::chrono::duration<double, std::milli>(16.74); // Each frame is ~16.74ms

    while (cpu_running)
    {
        auto frame_start = std::chrono::high_resolution_clock::now();
        
        gobby->tick_systems();

        auto frame_end = std::chrono::high_resolution_clock::now();

        if (auto elapsed = frame_end - frame_start; elapsed < frame_duration)
        {
            std::this_thread::sleep_for(frame_duration);
        }
    }

    return 0;
}