#ifndef __QARRANGEMENT_LABELLING_VANISHING_POINTS_WIDGET_H__
#define __QARRANGEMENT_LABELLING_VANISHING_POINTS_WIDGET_H__

#include "utilities/Singleton.hpp"

#include <QDockWidget>
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
       QActionGroup* _modeGroup;
        Ui::QArrangementLabellingVanishingPointsWidget *ui;
private:
    explicit QArrangementLabellingVanishingPointsWidget(QDockWidget *parent = 0);
    ~QArrangementLabellingVanishingPointsWidget();





};

#endif // __QARRANGEMENT_LABELLING_VANISHING_POINTS_WIDGET_H__
