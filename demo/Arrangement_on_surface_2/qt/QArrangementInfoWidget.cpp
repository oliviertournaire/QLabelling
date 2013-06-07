#include "QArrangementInfoWidget.h"
#include "ui_QArrangementInfoWidget.h"

QArrangementInfoWidget::QArrangementInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QArrangementInfoWidget)
{
    ui->setupUi(this);
}

QArrangementInfoWidget::~QArrangementInfoWidget()
{
    delete ui;
}

void QArrangementInfoWidget::setNumFaces(const int &numFaces, const int numUnboudedFaces)
{
    ui->_labelFaces->setText( QString::number(numFaces) + " faces (" + QString::number(numUnboudedFaces) + " unbounded)" );
}

void QArrangementInfoWidget::setNumEdges(const int &numEdges)
{
    ui->_labelEdges->setText( QString::number(numEdges) + " edges" );
}

void QArrangementInfoWidget::setNumVertices(const int &numVertices)
{
    ui->_labelVertices->setText( QString::number(numVertices) + " vertices" );
}

void QArrangementInfoWidget::setNumIsolatedVertices(const int &numIsolatedVertices)
{
    ui->_labelIsolatedVertices->setText( QString::number(numIsolatedVertices) + " isolated vertices" );
}

void QArrangementInfoWidget::setNumVerticesAtInfinity(const int &numVerticesAtInfinity)
{
    ui->_labelVerticesAtInfinity->setText( QString::number(numVerticesAtInfinity) + " vertices at infinity" );
}
