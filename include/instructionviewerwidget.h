#pragma once

#include <QObject>
#include <QTextEdit>
#include <QWidget>

class InstructionViewerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InstructionViewerWidget(QWidget *parent = nullptr);

public slots:
    void append_instruction(const std::string &instruction);

private:
    QTextEdit *instruction_list;
};