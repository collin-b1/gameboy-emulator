#pragma once

#include <QObject>
#include <QTextEdit>
#include <QWidget>

class OamViewerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OamViewerWidget(QWidget *parent = nullptr);
};