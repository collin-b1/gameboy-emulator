#pragma once

#include "core.h"
#include "debugwindow.h"
#include "mainwindow.h"
#include <QObject>

class EmulatorApp : public QObject
{
    Q_OBJECT
public:
    explicit EmulatorApp(QObject *parent = nullptr);
    void start(const std::string &rom_path = "");

public slots:
    void tick();
    void on_rom_loaded(const QString &path);

private:
    GameboyCore *core;
    QTimer *timer;
    MainWindow *main_window;
    DebugWindow *debug_window;
};