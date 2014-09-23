#include "QArrangementLabellingVanishingPointsWidget.h"
#include "ui_QArrangementLabellingVanishingPointsWidget.h"

#include "VanishingPoints.h"

#include <QActionGroup>
#include <QComboBox>

QArrangementLabellingVanishingPointsWidget::QArrangementLabellingVanishingPointsWidget(QDockWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::QArrangementLabellingVanishingPointsWidget)
{
    ui->setupUi(this);
    _indexVanishingPoint=-1;
    VanishingPoints::instance()->countervanishing=0;

    QObject::connect( this->ui->comboBox, SIGNAL(currentIndexChanged(int)),
                      this, SLOT  ( editIndexVanishingPoint(int) ));

    QObject::connect( this->ui->plusbutton, SIGNAL( clicked() ),
                      this, SLOT(addVanishingPoint() ) );

    QObject::connect( this->ui->lineEdit, SIGNAL( textEdited(QString) ),
                     this, SLOT(editComboBoxtext(QString) ) );

    QObject::connect(this->ui->comboBox,SIGNAL(currentIndexChanged(int)),
                     this,SLOT(numEdgesSlot()));
    //WIP end

}

QArrangementLabellingVanishingPointsWidget::~QArrangementLabellingVanishingPointsWidget()
{
    delete ui;
}

void QArrangementLabellingVanishingPointsWidget::editIndexVanishingPoint(int index)
{
    _indexVanishingPoint=index;

}
int QArrangementLabellingVanishingPointsWidget::GetIndexVanishingPoint(){
    return _indexVanishingPoint;
}
void QArrangementLabellingVanishingPointsWidget::numEdges(){
    if(_indexVanishingPoint==-1){
         ui->label_3->setText(tr("no vanishing point defines yet"));
        return;
    }
    if (VanishingPoints::instance()->EdgesVectorSize()==0){
        ui->label_3->setText(tr("no edges define yet"));
        return;
    }
    if(VanishingPoints::instance()->EdgesVectorSize()<_indexVanishingPoint+1){
        ui->label_3->setText(tr("no edges define yet"));
        return;
    }
    int num=VanishingPoints::instance()->EdgesSize(_indexVanishingPoint);
    ui->label_3->setText(tr("%n edge(s)","",num));
}
void QArrangementLabellingVanishingPointsWidget::numEdgesSlot(){
    numEdges();
}

int QArrangementLabellingVanishingPointsWidget::numPoints(){
    int num=ui->comboBox->count();
    return num;
}
void QArrangementLabellingVanishingPointsWidget::emitswitch(){
    emit switchvanishingmode();
}

void QArrangementLabellingVanishingPointsWidget::addVanishingPoint()
{


    int vanishingexisting=VanishingPoints::instance()->size();
    if(_indexVanishingPoint<vanishingexisting){
        int n=numPoints()+1;
        QString text="VanishingPoint";
        text.append(QString("%1").arg(n));
        this->ui->comboBox->addItem(text);
       this->ui->comboBox->setCurrentIndex (n-1);
         emitswitch();
    }
    else
        return;

    }

void QArrangementLabellingVanishingPointsWidget::removeVanishingTitle(unsigned int index){
    this->ui->comboBox->removeItem(index);
}

void QArrangementLabellingVanishingPointsWidget::editComboBoxtext(QString text){
   this->ui->comboBox->setItemText(GetIndexVanishingPoint(),text);
}

void QArrangementLabellingVanishingPointsWidget::updateTextCoordinates()
{
    double x, y;
    try
    {
        VanishingPoints::instance()->getVanishingPoints(_indexVanishingPoint, x, y);
    }
    catch (...)
    {
        ui->_xCoord->setText( tr("Unknown") );
        ui->_yCoord->setText( tr("Unknown") );

        return;
    }

    ui->_xCoord->setText( QString::number(x) );
    ui->_yCoord->setText( QString::number(y) );
}

//WIP
