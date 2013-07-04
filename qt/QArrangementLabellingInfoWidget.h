#ifndef __QARRANGEMENT_LABELLING_INFO_WIDGET_H__
#define __QARRANGEMENT_LABELLING_INFO_WIDGET_H__

#include "utilities/Singleton.hpp"

#include <QWidget>

namespace Ui {
class QArrangementLabellingInfoWidget;
}

class QArrangementLabellingInfoWidget : public QWidget, public Singleton<QArrangementLabellingInfoWidget>
{
    friend class Singleton<QArrangementLabellingInfoWidget>;
    Q_OBJECT
    
private:
    explicit QArrangementLabellingInfoWidget(QWidget *parent = 0);
    ~QArrangementLabellingInfoWidget();

public:
    void setNumFaces(const int &numFaces, const int numUnboudedFaces);
    void setNumEdges(const int &numEdges);
    void setNumVertices(const int &numVertices);
    void setNumIsolatedVertices(const int &numIsolatedVertices);
    void setNumVerticesAtInfinity(const int &numVerticesAtInfinity);
    void setChanged(const bool &changed);
    void reset();
    
private:
    Ui::QArrangementLabellingInfoWidget *ui;
};

#endif // __QARRANGEMENT_LABELLING_INFO_WIDGET_H__
