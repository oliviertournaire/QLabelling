#include "QArrangementInfoWidget.h"
#include "ui_QArrangementInfoWidget.h"
#include <QLabellingLogWidget.hpp>

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
    ui->_labelFaces->setText( tr("%n face(s)", "", numFaces) + tr(" (%n unbounded)","",numUnboudedFaces));
}

void QArrangementInfoWidget::setNumEdges(const int &numEdges)
{
    ui->_labelEdges->setText( tr("%n edge(s)","",numEdges ));
}

void QArrangementInfoWidget::setNumVertices(const int &numVertices)
{
    ui->_labelVertices->setText( tr("%n vertice(s)","",numVertices ));
}

void QArrangementInfoWidget::setNumIsolatedVertices(const int &numIsolatedVertices)
{
    ui->_labelIsolatedVertices->setText( tr("%n isolated vertice(s)","",numIsolatedVertices ));
}

void QArrangementInfoWidget::setNumVerticesAtInfinity(const int &numVerticesAtInfinity)
{
    ui->_labelVerticesAtInfinity->setText( tr("%n vertice(s) at infinity","",numVerticesAtInfinity));
}

void QArrangementInfoWidget::setChanged(const bool &changed)
{
    ui->_labelChanged->setText( changed ?tr( "Changes not saved") : tr("No new changes") );
}
