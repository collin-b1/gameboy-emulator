#include "debugwindow.h"
#include "definitions.h"
#include "tilemapviewerwidget.h"
#include <QTabWidget>
#include <QVBoxLayout>

DebugWindow::DebugWindow(QWidget *parent) : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);

    auto *tabs = new QTabWidget(this);

    // Tile Map Viewer
    tile_map_viewer = new TileMapViewerWidget(this);
    tabs->addTab(tile_map_viewer, "Tile Map Viewer");

    layout->addWidget(tabs);

    setLayout(layout);
    setWindowTitle("Gobby Debug Tools");
    // setMinimumSize(DEBUG_SCREEN_WIDTH * DEBUG_RENDER_SCALE, DEBUG_SCREEN_HEIGHT * DEBUG_RENDER_SCALE);
}

TileMapViewerWidget *DebugWindow::get_tile_map_viewer()
{
    return tile_map_viewer;
}
