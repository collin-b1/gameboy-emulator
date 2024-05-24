#include "gobby.h"
#include "cart.h"
#include "cpu.h"
#include "ppu.h"

#include <memory>
#include <iostream>

Gobby::Gobby() : 
    cart{},
    interrupts{},
    ppu(PPU{ interrupts }),
    mmu(MMU{ cart, ppu, interrupts }),
    cpu(CPU{ mmu, interrupts })
{}

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
        // Load CPU to post-boot state
        cpu.init_post_boot();

        // Disable Boot ROM
        mmu.bus_write(0xFF50, 0x01);
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
    bool game_loaded = gobby.load_game(rom_name, "dmg_boot.bin");

    if (!game_loaded)
    {
        std::cout << "Couldn't load ROM or Boot ROM." << std::endl;
    }

    bool cpu_running = true;
    while (cpu_running)
    {
        gobby.tick_systems();
    }

    return 0;
}