#pragma once

#include "register.h"
#include <QTableWidget>
#include <QTextEdit>
#include <QWidget>

class DebugWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DebugWindow(QWidget *parent = nullptr);

    // void updateRegisters(Registers &registers);
    // void updateMemory(std::array<uint8_t, 0x8000> &memory);

private:
    QTextEdit *registerText;
    QTableWidget *memoryTable;
};