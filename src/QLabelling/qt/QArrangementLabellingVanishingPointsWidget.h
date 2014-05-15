#ifndef __QARRANGEMENT_LABELLING_VANISHING_POINTS_WIDGET_H__
#define __QARRANGEMENT_LABELLING_VANISHING_POINTS_WIDGET_H__

#include "utilities/Singleton.hpp"

#include <QDockWidget>
#include <QComboBox>
class QActionGroup;

namespace Ui {
class QArrangementLabellingVanishingPointsWidget;
}

class QArrangementLabellingVanishingPointsWidget : public QDockWidget, public Singleton<QArrangementLabellingVanishingPointsWidget>
{
    friend class Singleton<QArrangementLabellingVanishingPointsWidget>;
    Q_OBJECT
public:
   void QCreateNewVanishingPoint_clicked(bool ckecked=false);
    void setupUi();
        Ui::QArrangementLabellingVanishingPointsWidget *ui;
        int GetIndexVanishingPoint();
        int numEdges();
        int numPoints();
        bool DrawnVanishing();
        void addVanishingPointMethod();
        void removeVanishingTitle(unsigned int index);




public slots:
    void editIndexVanishingPoint(int index);
    void addVanishingPoint();


private:
    explicit QArrangementLabellingVanishingPointsWidget(QDockWidget *parent = 0);
    ~QArrangementLabellingVanishingPointsWidget();
    int _indexVanishingPoint;





};

#endif // __QARRANGEMENT_LABELLING_VANISHING_POINTS_WIDGET_H__
