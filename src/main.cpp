#include "main.h"
#include "cart.h"
#include "cpu.h"
#include "ppu.h"

#include <iostream>

int main(int argc, char* argv[])
{
    std::string rom_name{"tetris.gb"};

    if (argc > 1)
    {
        rom_name = argv[1];
    }

    std::string rom_path{ ROMS_DIRECTORY + rom_name };

    InterruptManager imu{};
    PPU ppu{ imu };
    MemoryBus mem{ ppu, imu };
    CPU cpu{ mem, imu };

    bool rom_loaded = mem.get_cart().load_rom(rom_path);

    if (!rom_loaded)
    {
        std::cout << "Unable to load ROM: " << rom_name << std::endl;
        return 1;
    }
    
    if (rom_name != "dmg_boot.bin")
    {
        cpu.init();
    }

    bool cpu_running = true;
    while (cpu_running)
    {
        cpu.next_instruction();
    }

    return 0;
}