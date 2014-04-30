#include "QArrangementLabellingVanishingPointsWidget.h"
#include "ui_QArrangementLabellingVanishingPointsWidget.h"


QArrangementLabellingVanishingPointsWidget::QArrangementLabellingVanishingPointsWidget(QDockWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::QArrangementLabellingVanishingPointsWidget)
{
    ui->setupUi(this);

}

QArrangementLabellingVanishingPointsWidget::~QArrangementLabellingVanishingPointsWidget()
{
    delete ui;
}

