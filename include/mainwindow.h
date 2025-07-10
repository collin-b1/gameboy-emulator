#pragma once

#include "core.h"
#include "rendererwidget.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <QMainWindow>
#include <cstdint>
#include <memory>
#include <string>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(GameboyCore **core_ptr, QWidget *parent = nullptr);
    RendererWidget *get_renderer();

signals:
    void rom_loaded(const QString &path);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void on_load_rom_clicked();

private:
    RendererWidget *renderer;
    GameboyCore **core_ptr;
};