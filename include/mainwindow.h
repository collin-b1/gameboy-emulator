#pragma once

#include <cstdint>
#include <string>

#include "core.h"
#include "rendererwidget.h"
#include <QLabel>
#include <QMainWindow>

#include <memory>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    RendererWidget *get_renderer();

signals:
    void rom_loaded(const QString &path);

private slots:
    void on_load_rom_clicked();

private:
    RendererWidget *renderer;
};