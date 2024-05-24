#include "gobby.h"
#include "cart.h"
#include "cpu.h"
#include "ppu.h"
#include "renderer.h"

#include <memory>
#include <iostream>

Gobby::Gobby() : 
    cart{},
    interrupts{},
    renderer{},
    ppu(PPU{ interrupts }),
    mmu(MMU{ cart, ppu, interrupts }),
    cpu(CPU{ mmu, interrupts })
{}

bool Gobby::load_rom(std::string rom_name)
{
    std::string rom_path{ ROMS_DIRECTORY + rom_name };

    bool rom_loaded = cart.load_rom(rom_path);
    // cart.print_headers();

    if (!rom_loaded)
    {
        return false;
    }

    if (rom_name != "dmg_boot.bin")
    {
        cpu.init_post_boot();
    }

    return true;
}

void Gobby::tick_systems()
{
    cpu.next_instruction();
}

int main(int argc, char* argv[])
{
    std::string rom_name{"tetris.gb"};

    if (argc > 1)
    {
        rom_name = argv[1];
    }

    Gobby gobby{};
    bool rom_loaded = gobby.load_rom(rom_name);

    if (!rom_loaded)
    {
        std::cout << "Couldn't load rom." << std::endl;
    }

    bool cpu_running = true;
    while (cpu_running)
    {
        gobby.tick_systems();
    }

    return 0;
}