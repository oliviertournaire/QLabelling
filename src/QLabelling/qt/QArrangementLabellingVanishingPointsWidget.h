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
        unsigned int GetIndexVanishingPoint();
        void numEdges();
        int numPoints();
        void removeVanishingTitle(unsigned int index);
        void emitswitch();

signals:
        void switchvanishingmode();


public slots:
    void editIndexVanishingPoint(int index);
    void addVanishingPoint();
    void editComboBoxtext(QString text);
    void numEdgesSlot();


private:
    explicit QArrangementLabellingVanishingPointsWidget(QDockWidget *parent = 0);
    ~QArrangementLabellingVanishingPointsWidget();
    unsigned int _indexVanishingPoint;





};

#endif // __QARRANGEMENT_LABELLING_VANISHING_POINTS_WIDGET_H__
