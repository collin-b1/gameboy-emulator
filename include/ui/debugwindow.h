#pragma once

#include "QWidget"
#include "core.h"
#include "ui/oamviewerwidget.h"
#include "ui/tilemapviewerwidget.h"

class DebugWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DebugWindow(GameboyCore **core, QWidget *parent = nullptr);
    TileMapViewerWidget *get_tile_map_viewer();

private:
    GameboyCore **core_ptr;
    TileMapViewerWidget *tile_map_viewer;
    OamViewerWidget *oam_viewer;
};