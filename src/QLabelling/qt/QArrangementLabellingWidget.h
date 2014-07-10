#ifndef __QARRANGEMENT_LABELLING_WIDGET_H__
#define __QARRANGEMENT_LABELLING_WIDGET_H__

#include "utilities/Singleton.hpp"

#include <QWidget>

#include <vector>

class QGraphicsView;
class QLabelItem;
class QButtonGroup;

namespace Ui {
class QArrangementLabellingWidget; 
}

class QArrangementLabellingWidget : public QWidget, public Singleton<QArrangementLabellingWidget>
{
    friend class Singleton<QArrangementLabellingWidget>;
    Q_OBJECT
    
private:
    explicit QArrangementLabellingWidget();
    ~QArrangementLabellingWidget();

public:
//     QImage labelsImage();
    const std::vector<QLabelItem*>& labelItems() const { return _labelItems; }

    void setEnabledAllLabelButtons(bool enabled);

    void parseLabels();
    void setLabelsPath(const QString &labelsPath);
    inline QString const labelsPath() { return _labelsPath; }

    const QLabelItem* findActiveLabelItem() const;
    const QLabelItem* findLabelItemFromName(const QString &name) const;
    const QString Color2Label(const QColor color) const;
    const QColor Label2Color(const QString label) const;
    void addLabelPlus(QString labelname, QColor labelcolor);

public slots:
    void on__toolButtonChooseLabelsPath_clicked(bool checked = false);
    void addLabelChoice();
//     void on__radioButtonLabelling_toggled(bool checked);

    void updateAlphaValue(const int alpha);
 signals:
    void labelChanged();
    
protected:
    Ui::QArrangementLabellingWidget *ui;
    QString _stylesheetString;
    std::vector<QLabelItem*> _labelItems;
    QButtonGroup *_buttonGroup;
    QString _labelsPath;
};

#endif // __QARRANGEMENT_LABELLING_WIDGET_H__
