#include "mainwindow.h"
#include "definitions.h"

#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QMimeData>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    // Menu Bar
    auto *file_menu = menuBar()->addMenu(tr("File"));

    auto *load_rom_action = new QAction(tr("Load ROM from file"), this);
    file_menu->addAction(load_rom_action);
    connect(load_rom_action, &QAction::triggered, this, &MainWindow::on_load_rom_clicked);

    // Renderer
    renderer = new RendererWidget(central);
    layout->addWidget(renderer);

    central->setLayout(layout);
    setCentralWidget(central);

    // Allow dragging and dropping files onto the window (ROMs)
    setAcceptDrops(true);
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

        if (file_ext == "gb")
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

void MainWindow::on_load_rom_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Load ROM"), "", tr("Gameboy ROMs (*.gb)"));
    if (!file_path.isEmpty())
    {
        emit rom_loaded(file_path);
    }
}

RendererWidget *MainWindow::get_renderer()
{
    return renderer;
}
