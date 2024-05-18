#include "main.h"
#include "memory/cart.h"
#include "cpu/cpu.h"

int main()
{
	MemoryBus mem;
	mem.get_cart().load_rom(ROMS_DIRECTORY + std::string{"/dmg_boot.bin"});

	CPU cpu{ mem };
	//cpu.init();

	bool cpu_running = true;
	while (cpu_running)
	{
		cpu.next_instruction();
	}

	return 0;
}