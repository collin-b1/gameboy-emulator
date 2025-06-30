#include "emulatorapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    EmulatorApp emulator;

    if (argc > 1)
    {
        emulator.start(argv[1]);
    }
    else
    {
        emulator.start();
    }

    // Enter main loop
    return app.exec();
}