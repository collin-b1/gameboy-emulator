#include "debug.h"
#include <QVBoxLayout>

DebugWindow::DebugWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Debug");
    resize(600, 400);

    auto *layout = new QVBoxLayout(this);

    registerText = new QTextEdit(this);
    registerText->setReadOnly(true);
    layout->addWidget(registerText);

    memoryTable = new QTableWidget(this);
    memoryTable->setColumnCount(17);
    QStringList headers;
    headers << "Address";
    for (auto i{0}; i < 16; ++i)
    {
        headers << QString("%1").arg(i, 2, 16, QLatin1Char('0')).toUpper();
    }
    memoryTable->setHorizontalHeaderLabels(headers);
    layout->addWidget(memoryTable);
}