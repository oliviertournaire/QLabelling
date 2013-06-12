#ifndef QLABELLINGWIDGET_HPP
#define QLABELLINGWIDGET_HPP

#include "utilities/Singleton.hpp"

#include <QWidget>

#include <vector>

class QLabellingView;
class QGraphicsView;
class QLabelItem;
class QButtonGroup;

namespace Ui {
class QLabellingWidget;
}

class QLabellingWidget : public QWidget, public Singleton<QLabellingWidget>
{
    friend class Singleton<QLabellingWidget>;
    Q_OBJECT
    
private:
    explicit QLabellingWidget();
    ~QLabellingWidget();

public:
    QImage labelsImage();
    const std::vector<QLabelItem*>& labelItems() const { return _labelItems; }

    void setEnabledAllLabelButtons(bool enabled);

    void parseLabels();
    void setLabelsPath(const QString &labelsPath);

    const QLabelItem* findActiveLabelItem() const;
    const QLabelItem* findLabelItemFromName(const QString &name) const;

public slots:
    void on__toolButtonChooseLabelsPath_clicked(bool checked = false);
//     void on__radioButtonLabelling_toggled(bool checked);

    void updateAlphaValue(const int alpha);
 signals:
    void labelChanged();
    
protected:
    Ui::QLabellingWidget *ui;
    QString _stylesheetString;
    std::vector<QLabelItem*> _labelItems;
    QButtonGroup *_buttonGroup;
    QString _labelsPath;
};

#endif // QLABELLINGWIDGET_HPP
