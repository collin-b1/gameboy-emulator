#include "main.h"
#include "memory/cart.h"
#include "cpu/cpu.h"

int main(int argc, char **argv)
{
    std::string rom_name{"06-ld r,r.gb"}; // 06-ld r,r.gb

    if (argc > 1)
    {
        rom_name = argv[1];
    }

    std::string rom_path{ ROMS_DIRECTORY + rom_name };

    MemoryBus mem;
    mem.get_cart().load_rom(rom_path);

    CPU cpu{ mem };
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