#include "gobby.h"
#include "definitions.h"
#include "ppu.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QTimer>
#include <iostream>
#include <thread>

Gobby::Gobby(QMainWindow *window)
    : clock(0)
    , cart{}
    , interrupts{}
    , timer{interrupts}
    , renderer{}
    , debug{}
    , ppu{interrupts, renderer}
    , mmu{cart, ppu, interrupts, timer}
    , cpu{mmu, interrupts}
{
    ppu.bind_mmu(&mmu);
    window->setCentralWidget(&renderer);
}

auto Gobby::load_game(const std::string &rom_name, const std::string &boot_rom) -> bool
{
    std::string rom_path{rom_name};
    std::string boot_rom_path{boot_rom};

    bool rom_loaded = cart.load_rom(rom_path);
    bool boot_rom_loaded = cart.load_boot_rom(boot_rom_path);

    if (!rom_loaded || !boot_rom_loaded)
    {
        return false;
    }

    cart.print_headers();

    return true;
}

void Gobby::tick_systems()
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

        // std::cout << cycles << " " << cpu.get_state() << std::endl;
    }
}
Renderer *Gobby::get_renderer_widget()
{
    return &renderer;
}
QImage Gobby::get_tile_map_image() const
{
    return ppu.generate_tile_map();
}

auto main(int argc, char *argv[]) -> int
{
    QApplication app(argc, argv);
    QMainWindow window;

    std::string rom_path{R"(roms\tetris.gb)"};
    std::string boot_rom_path{R"(roms\dmg_boot.bin)"};

    if (argc > 1)
    {
        rom_path = argv[1];
    }

    const auto gobby = std::make_unique<Gobby>(&window);
    if (!gobby->load_game(rom_path, boot_rom_path))
    {
        std::cout << "Rom not found!" << std::endl;
        exit(1);
    }

    auto *main_screen = gobby->get_renderer_widget();
    auto *tile_map_screen = new DebugWindow;

    auto *central = new QWidget;
    auto *layout = new QHBoxLayout(central);

    layout->addWidget(main_screen, 0);
    layout->addWidget(tile_map_screen, 1);
    window.setCentralWidget(central);

    window.show();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        gobby->tick_systems();
        tile_map_screen->set_tile_map(gobby->get_tile_map_image());
    });
    timer.start(16);

    // Enter game loop
    return app.exec();
}