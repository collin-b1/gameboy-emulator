#include "ui/debugwindow.h"
#include "definitions.h"
#include <QTabWidget>
#include <QVBoxLayout>

DebugWindow::DebugWindow(GameboyCore **core_ptr, QWidget *parent) : QWidget(parent), core_ptr(core_ptr)
{
    auto *layout = new QVBoxLayout(this);

    auto *tabs = new QTabWidget(this);

    // Tile Map Viewer
    tile_map_viewer = new TileMapViewerWidget(this);
    tabs->addTab(tile_map_viewer, "Tiles");

    // Instructions Viewer
    oam_viewer = new OamViewerWidget(this);
    tabs->addTab(oam_viewer, "OAM");

    layout->addWidget(tabs);
    setLayout(layout);
    setWindowTitle("Gobby Debug Tools");
}

TileMapViewerWidget *DebugWindow::get_tile_map_viewer()
{
    return tile_map_viewer;
}
