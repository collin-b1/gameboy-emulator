#include "ui/oamviewerwidget.h"
#include <QVBoxLayout>

OamViewerWidget::OamViewerWidget(QWidget *parent) : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    setLayout(layout);
}