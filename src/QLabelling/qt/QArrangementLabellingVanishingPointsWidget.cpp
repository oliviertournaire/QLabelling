#include "QArrangementLabellingVanishingPointsWidget.h"
#include "ui_QArrangementLabellingVanishingPointsWidget.h"
#include <QActionGroup>
#include "VanishingPoints.h"


QArrangementLabellingVanishingPointsWidget::QArrangementLabellingVanishingPointsWidget(QDockWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::QArrangementLabellingVanishingPointsWidget)
{
    ui->setupUi(this);
    VanishingPoints::instance()->countervanishing=0;

}

QArrangementLabellingVanishingPointsWidget::~QArrangementLabellingVanishingPointsWidget()
{
    delete ui;
}
//MAXI WIP
//        QAction QArrangementLabellingVanishingPointsWidget::QCreateNewVanishingPoint_clicked(bool checked){
//             Q_UNUSED(checked);
//            return actionEdit_VanishingPoint;
//        }
