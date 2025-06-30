#include "instructionviewerwidget.h"
#include <QVBoxLayout>

InstructionViewerWidget::InstructionViewerWidget(QWidget *parent) : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    instruction_list = new QTextEdit(this);
    instruction_list->setReadOnly(true);
    layout->addWidget(instruction_list);
    setLayout(layout);
}

void InstructionViewerWidget::append_instruction(const std::string &instruction)
{
    instruction_list->append(QString::fromStdString(instruction));
}