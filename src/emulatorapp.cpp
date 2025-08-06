#include "emulatorapp.h"
#include "ui/debugwindow.h"
#include "ui/mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <iostream>

EmulatorApp::EmulatorApp(QObject *parent) : QObject(parent)
{
    core = nullptr;
    main_window = new MainWindow(&core);
    debug_window = new DebugWindow(&core);

    // Render loop
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &EmulatorApp::tick);

    connect(main_window, &MainWindow::rom_loaded, this, &EmulatorApp::on_rom_loaded);
}

void EmulatorApp::start(const std::string &rom_path)
{
    main_window->show();

    if (!rom_path.empty())
    {
        std::cout << "Loaded ROM from args: " << rom_path << std::endl;
        emit on_rom_loaded(QString::fromStdString(rom_path));
    }
}

void EmulatorApp::tick()
{
    core->tick_systems();
}

void EmulatorApp::on_rom_loaded(const QString &path)
{
    delete core;
    core = new GameboyCore();

    if (!core->load_rom(path.toStdString()))
    {
        QMessageBox::warning(nullptr, tr("Error"), tr("Failed to load ROM file"));
        return;
    }

    core->get_ppu().set_frame_listener(main_window->get_renderer());
    core->get_ppu().set_tilemap_listener(debug_window->get_tile_map_viewer());

    debug_window->show();

    timer->start(16);
}