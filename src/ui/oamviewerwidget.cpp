#include "ui/oamviewerwidget.h"
#include <QHeaderView>
#include <QVBoxLayout>

OamViewerWidget::OamViewerWidget(QWidget *parent) : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    setLayout(layout);

    oam_table = new QTableWidget(this);

    oam_table->setRowCount(40);
    oam_table->setColumnCount(4);

    oam_table->setHorizontalHeaderLabels({"Y", "X", "Tile", "Flags"});

    for (auto row{0}; row < oam_table->rowCount(); ++row)
    {
        for (auto col{0}; col < oam_table->columnCount(); ++col)
        {
            auto *item = new QTableWidgetItem();
            oam_table->setItem(row, col, item);
        }
    }

    oam_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    oam_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addWidget(oam_table);
}

void OamViewerWidget::on_frame_ready(const std::array<Object, 40> &sprites)
{
    for (auto i{0}; i < 40; ++i)
    {
        auto obj = sprites[i];

        oam_table->item(i, 0)->setText(QString::number(obj.y));
        oam_table->item(i, 1)->setText(QString::number(obj.x));
        oam_table->item(i, 2)->setText(QString::number(obj.tile_index));
        oam_table->item(i, 3)->setText(QString::number(obj.flags.flags));
    }
}
