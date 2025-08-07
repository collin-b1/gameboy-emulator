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

#ifdef __EMSCRIPTEN__
#include <QPushButton>
#include <QStatusBar>
#endif

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

    // Menu Bar
#ifndef __EMSCRIPTEN__
    auto *file_menu = menuBar()->addMenu(tr("File"));
    auto *load_rom_action = new QAction(tr("Load ROM from file"), this);
    file_menu->addAction(load_rom_action);

    connect(load_rom_action, &QAction::triggered, this, &MainWindow::on_load_rom_clicked);

    // Allow dragging and dropping files onto the window (ROMs)
    setAcceptDrops(true);
#else
    auto *load_rom_button = new QPushButton("Load ROM", this);
    connect(load_rom_button, &QPushButton::clicked, this, &MainWindow::on_load_rom_clicked);
    statusBar()->addPermanentWidget(load_rom_button);
#endif

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
#ifdef __EMSCRIPTEN__
    trigger_wasm_file_dialog();
#else
    QString file_path = QFileDialog::getOpenFileName(this, tr("Load ROM"), "", tr("Gameboy ROMs (*.gb *.gbc)"));
    if (!file_path.isEmpty())
    {
        emit rom_loaded(file_path);
    }
#endif
}

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>

void MainWindow::trigger_wasm_file_dialog()
{
    EM_ASM({
        if (!Module.romInput)
        {
            const input = document.createElement('input');
            input.type = 'file';
            input.accept = '.gb,.gbc'; // Optional: restrict file types
            input.style.display = 'none';
            input.onchange = (e) =>
            {
                const file = e.target.files[0];
                const reader = new FileReader();
                reader.onload = () =>
                {
                    const buffer = new Uint8Array(reader.result);
                    Module.ccall('handle_rom_upload', null, [ 'array', 'number' ], [ buffer, buffer.length ]);
                };
                reader.readAsArrayBuffer(file);
            };
            document.body.appendChild(input);
            Module.romInput = input;
        }
        Module.romInput.click();
    });
}

extern "C" EMSCRIPTEN_KEEPALIVE void handle_rom_upload(const uint8_t *data, int size)
{
    QByteArray rom_data(reinterpret_cast<const char *>(data), size);

    // Safely access the MainWindow from Qt's main thread
    QMetaObject::invokeMethod(qApp, [rom_data]() {
        MainWindow *window = qobject_cast<MainWindow *>(qApp->activeWindow());
        /*if (window && window->core_ptr && window->core_ptr[0])
        {
            window->core_ptr[0]->loadROM(rom_data);
        }*/
    });
}
#endif

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
