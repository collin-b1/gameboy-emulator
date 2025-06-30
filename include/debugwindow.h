#pragma once

#include "QWidget"
#include "instructionviewerwidget.h"
#include "tilemapviewerwidget.h"

class DebugWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DebugWindow(QWidget *parent = nullptr);
    TileMapViewerWidget *get_tile_map_viewer();

private:
    TileMapViewerWidget *tile_map_viewer;
    InstructionViewerWidget *instruction_viewer;
};