#pragma once

#include "ui/framelistener.h"
#include <QObject>
#include <QTableWidget>
#include <QTextEdit>
#include <QWidget>

class OamViewerWidget : public QWidget, public ISpriteFrameListener
{
    Q_OBJECT
public:
    explicit OamViewerWidget(QWidget *parent = nullptr);

    void on_frame_ready(const std::array<Object, 40> &sprites) override;

private:
    QTableWidget *oam_table;
};