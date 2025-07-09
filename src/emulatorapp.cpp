#include "emulatorapp.h"
#include "debugwindow.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <iostream>

EmulatorApp::EmulatorApp(QObject *parent) : QObject(parent)
{
    core = nullptr;
    main_window = new MainWindow(&core);
    debug_window = new DebugWindow();

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
    core = new GameboyCore(this);

    if (!core->load_rom(path.toStdString()))
    {
        QMessageBox::warning(nullptr, tr("Error"), tr("Failed to load ROM file"));
        return;
    }

    connect(core->get_ppu(), &PPU::frame_ready, main_window->get_renderer(), &RendererWidget::update_frame);
    connect(core->get_ppu(), &PPU::tile_map_ready, debug_window->get_tile_map_viewer(),
            &TileMapViewerWidget::update_tile_map);

    debug_window->show();

    timer->start(16);
}