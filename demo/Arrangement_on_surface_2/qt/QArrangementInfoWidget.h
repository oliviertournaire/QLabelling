#ifndef QARRANGEMENTINFOWIDGET_H
#define QARRANGEMENTINFOWIDGET_H

#include "utilities/Singleton.hpp"

#include <QWidget>

namespace Ui {
class QArrangementInfoWidget;
}

class QArrangementInfoWidget : public QWidget, public Singleton<QArrangementInfoWidget>
{
    friend class Singleton<QArrangementInfoWidget>;
    Q_OBJECT
    
private:
    explicit QArrangementInfoWidget(QWidget *parent = 0);
    ~QArrangementInfoWidget();

public:
    void setNumFaces(const int &numFaces, const int numUnboudedFaces);
    void setNumEdges(const int &numEdges);
    void setNumVertices(const int &numVertices);
    void setNumIsolatedVertices(const int &numIsolatedVertices);
    void setNumVerticesAtInfinity(const int &numVerticesAtInfinity);
    
private:
    Ui::QArrangementInfoWidget *ui;
};

#endif // QARRANGEMENTINFOWIDGET_H
