#ifndef __QARRANGEMENT_LABELLING_VANISHING_POINTS_WIDGET_H__
#define __QARRANGEMENT_LABELLING_VANISHING_POINTS_WIDGET_H__

#include "utilities/Singleton.hpp"

#include <QDockWidget>

namespace Ui {
class QArrangementLabellingVanishingPointsWidget;
}

class QArrangementLabellingVanishingPointsWidget : public QWidget, public Singleton<QArrangementLabellingVanishingPointsWidget>
{
    friend class Singleton<QArrangementLabellingVanishingPointsWidget>;
    Q_OBJECT

private:
    explicit QArrangementLabellingVanishingPointsWidget(QWidget *parent = 0);
    ~QArrangementLabellingVanishingPointsWidget();

public:


private:
    Ui::QArrangementLabellingVanishingPointsWidget *ui;
};

#endif // __QARRANGEMENT_LABELLING_VANISHING_POINTS_WIDGET_H__
