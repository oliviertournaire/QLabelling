#ifndef QCGALARRANGEMENTMAINWINDOW_HPP
#define QCGALARRANGEMENTMAINWINDOW_HPP

#include "ui_QCGALArrangementMainWindow.h"

namespace CGAL {
namespace Qt {
    template <typename T> class ArrangementGraphicsItem;
} // Qt
} // CGAL

// Qt
#include <QMainWindow>
#include <QGraphicsScene>

class QLabel;

// CGAL
#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>

typedef CGAL::Quotient<CGAL::MP_Float> Number_type;
typedef CGAL::Cartesian<Number_type> Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel> Traits_2;
typedef Traits_2::Point_2 Point_2;
typedef Traits_2::X_monotone_curve_2 Segment_2;
typedef CGAL::Arrangement_2<Traits_2> Arrangement_2;

class QCGALArrangementMainWindow : public QMainWindow, public Ui::QCGALArrangementMainWindow
{
    Q_OBJECT
    
public:
    explicit QCGALArrangementMainWindow(QWidget *parent = 0);

public slots:
    void on_actionAddPoint_triggered();
    void on_actionAddLine_triggered();
    
protected:
    Arrangement_2 _currentArrangement;
    CGAL::Qt::ArrangementGraphicsItem<Arrangement_2> *_currentArrangementGraphicsIem;
    QGraphicsScene _scene;
    QLabel *_labelStatus;
};

#endif // QCGALARRANGEMENTMAINWINDOW_HPP
