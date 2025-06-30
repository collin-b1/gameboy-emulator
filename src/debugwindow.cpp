#include "debugwindow.h"
#include "definitions.h"
#include <QTabWidget>
#include <QVBoxLayout>

DebugWindow::DebugWindow(QWidget *parent) : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);

    auto *tabs = new QTabWidget(this);

    // Tile Map Viewer
    tile_map_viewer = new TileMapViewerWidget(this);
    tabs->addTab(tile_map_viewer, "Tile Map");

    // Instructions Viewer
    instruction_viewer = new InstructionViewerWidget(this);
    tabs->addTab(instruction_viewer, "Instructions");

    layout->addWidget(tabs);
    setLayout(layout);
    setWindowTitle("Gobby Debug Tools");
}

TileMapViewerWidget *DebugWindow::get_tile_map_viewer()
{
    return tile_map_viewer;
}
