#include "main.h"
#include "memory/cart.h"
#include "cpu/cpu.h"

int main(int argc, char **argv)
{
    std::string rom_name{"dmg_boot.bin"};

    if (argc > 1)
    {
        rom_name = argv[1];
    }

    std::string rom_path{ ROMS_DIRECTORY + rom_name };

    MemoryBus mem;
    mem.get_cart().load_rom(rom_path);

    CPU cpu{ mem };
    if (argc > 1)
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