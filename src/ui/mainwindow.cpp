#include "ui/mainwindow.h"
#include "definitions.h"
#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QMimeData>
#include <QSplitter>
#include <QVBoxLayout>
#include <iostream>

MainWindow::MainWindow(GameboyCore **core_ptr, QWidget *parent) : QMainWindow(parent), core_ptr(core_ptr)
{
    auto *splitter = new QSplitter(Qt::Horizontal);

    // Widgets
    renderer = new RendererWidget(this);
    renderer->installEventFilter(this);
    renderer->setFocusPolicy(Qt::StrongFocus);

    debug_widget = new DebugWidget(core_ptr);

    splitter->addWidget(renderer);
    splitter->addWidget(debug_widget);

    splitter->setStretchFactor(0, 3); // renderer gets more space
    splitter->setStretchFactor(1, 2); // debug window less

    // menuBar()->setContentsMargins(0, 0, 0, 0);

    // Menu Bar
    auto *file_menu = menuBar()->addMenu(tr("File"));
    auto *load_rom_action = new QAction(tr("Load ROM from file"), this);
    file_menu->addAction(load_rom_action);

    connect(load_rom_action, &QAction::triggered, this, &MainWindow::on_load_rom_clicked);
    
    auto *emulation_menu = menuBar()->addMenu(tr("Emulation"));
    auto *pause_action = new QAction(tr("Pause/Resume"), this);
    emulation_menu->addAction(pause_action);

    // Allow dragging and dropping files onto the window (ROMs)
    setAcceptDrops(true);

    setCentralWidget(splitter);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (event->mimeData()->hasUrls() && urls.size() == 1)
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (urls.size() == 1)
    {
        QString file_path = urls[0].toLocalFile();
        QFileInfo file_info(file_path);
        QString file_ext = file_info.suffix().toLower();

        if (file_ext == "gb" || file_ext == "gbc")
        {
            emit rom_loaded(file_path);

            event->acceptProposedAction();
        }
        else
        {
            event->ignore();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QApplication::quit();
    QMainWindow::closeEvent(event); // or QWidget::closeEvent(event);
};

void MainWindow::on_load_rom_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Load ROM"), "", tr("Gameboy ROMs (*.gb *.gbc)"));
    if (!file_path.isEmpty())
    {
        emit rom_loaded(file_path);
    }
}

void MainWindow::on_pause_emulator_clicked()
{
    // core_ptr
}

RendererWidget *MainWindow::get_renderer()
{
    return renderer;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return;
    }

    auto key = event->key();
    if ((*core_ptr) != nullptr)
    {
        switch (key)
        {
        case Qt::Key_Up:
            (*core_ptr)->set_button_pressed(Gobby::UP, true);
            break;
        case Qt::Key_Shift:
            (*core_ptr)->set_button_pressed(Gobby::SELECT, true);
            break;
        case Qt::Key_Down:
            (*core_ptr)->set_button_pressed(Gobby::DOWN, true);
            break;
        case Qt::Key_Return:
            (*core_ptr)->set_button_pressed(Gobby::START, true);
            break;
        case Qt::Key_X:
            (*core_ptr)->set_button_pressed(Gobby::B, true);
            break;
        case Qt::Key_Left:
            (*core_ptr)->set_button_pressed(Gobby::LEFT, true);
            break;
        case Qt::Key_Z:
            (*core_ptr)->set_button_pressed(Gobby::A, true);
            break;
        case Qt::Key_Right:
            (*core_ptr)->set_button_pressed(Gobby::RIGHT, true);
            break;
        }
    }
    else
    {
        std::cout << "Core not loaded!" << std::endl;
    }

    QWidget::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return;
    }

    if ((*core_ptr) != nullptr)
    {
        auto key = event->key();
        switch (key)
        {
        case Qt::Key_Up:
            (*core_ptr)->set_button_pressed(Gobby::UP, false);
            break;
        case Qt::Key_Shift:
            (*core_ptr)->set_button_pressed(Gobby::SELECT, false);
            break;
        case Qt::Key_Down:
            (*core_ptr)->set_button_pressed(Gobby::DOWN, false);
            break;
        case Qt::Key_Return:
            (*core_ptr)->set_button_pressed(Gobby::START, false);
            break;
        case Qt::Key_X:
            (*core_ptr)->set_button_pressed(Gobby::B, false);
            break;
        case Qt::Key_Left:
            (*core_ptr)->set_button_pressed(Gobby::LEFT, false);
            break;
        case Qt::Key_Z:
            (*core_ptr)->set_button_pressed(Gobby::A, false);
            break;
        case Qt::Key_Right:
            (*core_ptr)->set_button_pressed(Gobby::RIGHT, false);
            break;
        }
    }

    QWidget::keyPressEvent(event);
}
DebugWidget *MainWindow::get_debug_widget()
{
    return this->debug_widget;
}
