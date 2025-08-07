#pragma once

#include "QWidget"
#include "core.h"
#include "ui/oamviewerwidget.h"
#include "ui/tilemapviewerwidget.h"

class DebugWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DebugWidget(GameboyCore **core, QWidget *parent = nullptr);
    TileMapViewerWidget *get_tile_map_viewer();
    OamViewerWidget *get_oam_viewer();

private:
    GameboyCore **core_ptr;
    TileMapViewerWidget *tile_map_viewer;
    OamViewerWidget *oam_viewer;
};