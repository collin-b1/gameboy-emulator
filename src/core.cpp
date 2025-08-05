#include "core.h"

GameboyCore::GameboyCore(QObject *parent)
    : QObject(parent)
    , clock(0)
    , cart{}
    , interrupts{}
    , joypad{interrupts}
    , timer{interrupts}
    , ppu{interrupts}
    , mmu{cart, ppu, interrupts, timer, joypad}
    , cpu{mmu, interrupts}
{
}

void GameboyCore::tick_systems()
{
    unsigned int cycles{0};
    while (cycles < CYCLES_PER_FRAME)
    {
        const u8 cpu_cycles = cpu.next_instruction();
        cycles += cpu_cycles;

        // Timer tick
        timer.tick(cpu_cycles);

        // PPU tick
        ppu.tick(cpu_cycles);
    }
}

bool GameboyCore::load_rom(const std::string &rom_path)
{
    std::string boot_rom_path{R"(roms\dmg_boot.bin)"};

    bool rom_loaded = cart.load_rom(rom_path);
    bool boot_rom_loaded = cart.load_boot_rom(boot_rom_path);

    if (!rom_loaded)
    {
        return false;
    }

    if (!boot_rom_loaded)
    {
        cpu.init_post_boot();
    }

    cart.print_headers();

    return true;
}

PPU &GameboyCore::get_ppu()
{
    return ppu;
}

void GameboyCore::set_button_pressed(Gobby::JoypadInput button, bool pressed)
{
    joypad.set_button_pressed(button, pressed);
}
