#include "QArrangementLabellingInfoWidget.h"
#include "ui_QArrangementLabellingInfoWidget.h"
#include "QArrangementLabellingLogWidget.h"

QArrangementLabellingInfoWidget::QArrangementLabellingInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QArrangementLabellingInfoWidget)
{
    ui->setupUi(this);
}

QArrangementLabellingInfoWidget::~QArrangementLabellingInfoWidget()
{
    delete ui;
}

void QArrangementLabellingInfoWidget::setNumFaces(const int &numFaces, const int numUnboudedFaces)
{
    ui->_labelFaces->setText( tr("%n face(s)", "", numFaces) + tr(" (%n unbounded)","",numUnboudedFaces));
}

void QArrangementLabellingInfoWidget::setNumEdges(const int &numEdges)
{
    ui->_labelEdges->setText( tr("%n edge(s)","",numEdges ));
}

void QArrangementLabellingInfoWidget::setNumVertices(const int &numVertices)
{
    ui->_labelVertices->setText( tr("%n vertice(s)","",numVertices ));
}

void QArrangementLabellingInfoWidget::setNumIsolatedVertices(const int &numIsolatedVertices)
{
    ui->_labelIsolatedVertices->setText( tr("%n isolated vertice(s)","",numIsolatedVertices ));
}
void QArrangementLabellingInfoWidget::setFaceLabel(QString t)
{
    ui->_labelCurrentFaceLabel->setText(t);
}
QString QArrangementLabellingInfoWidget::FaceLabel()
{   QString t;
    if (ui->_labelCurrentFaceLabel->text()==NULL)
        t="";
    else
        t=ui->_labelCurrentFaceLabel->text();
    return t;
}

void QArrangementLabellingInfoWidget::setNumVerticesAtInfinity(const int &numVerticesAtInfinity)
{
    ui->_labelVerticesAtInfinity->setText( tr("%n vertice(s) at infinity","",numVerticesAtInfinity));
}

void QArrangementLabellingInfoWidget::setChanged(const bool &changed)
{
    ui->_labelChanged->setText( changed ?tr( "Changes not saved") : tr("No new changes") );
}

void QArrangementLabellingInfoWidget::reset()
{
    setNumFaces             ( 0 , 0 );
    setNumEdges             ( 0 );
    setNumVertices          ( 0 );
    setNumIsolatedVertices  ( 0 );
    setNumVerticesAtInfinity( 0 );
    setChanged              ( false );
}
