#include "gobby.h"
#include "ppu.h"

#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include <QApplication>
#include <QMainWindow>
#include <QTimer>

Gobby::Gobby(QMainWindow* window)
    : clock(0)
    , cart{}
    , interrupts{}
    , timer{ interrupts }
    , renderer{}
    , ppu{ interrupts, renderer }
    , mmu{ cart, ppu, interrupts, timer }
    , cpu{ mmu, interrupts }
{
    window->setCentralWidget(&renderer);
}

auto Gobby::load_game(const std::string& rom_name, const std::string& boot_rom) -> bool
{
    std::string rom_path{ rom_name };
    std::string boot_rom_path{ boot_rom };

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

auto main(int argc, char *argv[]) -> int
{
    QApplication app(argc, argv);
    QMainWindow window;

    std::string rom_path{ R"(roms\snake.gb)" };
    std::string boot_rom_path{R"(roms\dmg_boot.bin)"};

    if (argc > 1)
    {
        rom_path = argv[1];
    }

    const auto gobby = std::make_unique<Gobby>(&window);
    if (!gobby->load_game(rom_path, boot_rom_path)) {
        std::cout << "Rom not found!" << std::endl;
        exit(1);
    }

    window.show();

//    bool cpu_running = true;
//    const auto frame_duration = std::chrono::duration<double, std::milli>(16.74); // Each frame is ~16.74ms



//    while (cpu_running)
//    {
//        auto frame_start = std::chrono::high_resolution_clock::now();
//
//        gobby->tick_systems();
//
//        auto frame_end = std::chrono::high_resolution_clock::now();
//
//        if (auto elapsed = frame_end - frame_start; elapsed < frame_duration)
//        {
//            std::this_thread::sleep_for(frame_duration);
//        }
//    }

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        gobby->tick_systems();
        window.update();
    });
    timer.start(16);

    // Enter game loop
    return app.exec();
}