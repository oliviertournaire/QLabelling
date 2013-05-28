#include "QCGALArrangementMainWindow.hpp"
#include "CGAL/Qt/ArrangementGraphicsItem.hpp"

using namespace CGAL::Qt;

// Qt
#include <QLabel>

QCGALArrangementMainWindow::QCGALArrangementMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    _labelStatus = new QLabel(_statusBar);
    _statusBar->addPermanentWidget(_labelStatus, 200);
    _labelStatus->setText("QCGALArrangement");

    _currentArrangementGraphicsIem = new ArrangementGraphicsItem<Arrangement_2>(&_currentArrangement);
}

void QCGALArrangementMainWindow::on_actionAddPoint_triggered()
{
    _labelStatus->setText("on_actionAddPoint_toggled");
    //CGAL::insert_point(_currentArrangement, Point_2(rand(),rand()) );
    _labelStatus->setText( "Arrangement contains " + QString::number(_currentArrangement.number_of_vertices()) + " vertices");
}

void QCGALArrangementMainWindow::on_actionAddLine_triggered()
{
    _labelStatus->setText("on_actionAddLine_toggled");
}
