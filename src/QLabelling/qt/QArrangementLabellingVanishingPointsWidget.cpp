#include "QArrangementLabellingVanishingPointsWidget.h"
#include "ui_QArrangementLabellingVanishingPointsWidget.h"
#include <QActionGroup>
#include "VanishingPoints.h"
#include <QComboBox>



QArrangementLabellingVanishingPointsWidget::QArrangementLabellingVanishingPointsWidget(QDockWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::QArrangementLabellingVanishingPointsWidget)
{
    ui->setupUi(this);
    _indexVanishingPoint=0;
    VanishingPoints::instance()->countervanishing=0;

    QObject::connect( this->ui->comboBox, SIGNAL(currentIndexChanged(int)),
                      this, SLOT  ( editIndexVanishingPoint(int) ));
    //WIP end

}

QArrangementLabellingVanishingPointsWidget::~QArrangementLabellingVanishingPointsWidget()
{
    delete ui;
}
void QArrangementLabellingVanishingPointsWidget::editIndexVanishingPoint(int index){
    _indexVanishingPoint=index;
}
int QArrangementLabellingVanishingPointsWidget::GetIndexVanishingPoint(){
    return _indexVanishingPoint;
}
int QArrangementLabellingVanishingPointsWidget::numEdges(){
    int num=VanishingPoints::instance()->EdgesSize(GetIndexVanishingPoint());
    return num;
}
int QArrangementLabellingVanishingPointsWidget::numPoints(){
    int num=ui->comboBox->count();
    return num;
}
bool QArrangementLabellingVanishingPointsWidget::DrawnVanishing(){
    if (numEdges()<2)
        return false;
    else
        return true;
}
void QArrangementLabellingVanishingPointsWidget::addVanishingPoint(){
        int n=numPoints()+1;
        QString text="VanishingPoint";
        text.append(QString("%1").arg(n));
        this->ui->comboBox->addItem(text);
       this->ui->comboBox->setCurrentIndex (n-1);

    }
void QArrangementLabellingVanishingPointsWidget::addVanishingPointMethod(){
QObject::connect( this->ui->plusbutton, SIGNAL( clicked() ),
                  this, SLOT(addVanishingPoint() ) );
}
void QArrangementLabellingVanishingPointsWidget::removeVanishingTitle(unsigned int index){
    this->ui->comboBox->removeItem(index);
}

//WIP
