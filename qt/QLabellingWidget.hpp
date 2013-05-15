#ifndef QLABELLINGWIDGET_HPP
#define QLABELLINGWIDGET_HPP

#include <QWidget>

#include <vector>

class QLabellingView;
class QGraphicsView;
class QLabelItem;
class QButtonGroup;

namespace Ui {
class QLabellingWidget;
}

class QLabellingWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit QLabellingWidget(QWidget *parent = 0);
    ~QLabellingWidget();

    QLabellingView* view();
    QImage labelsImage();
    const std::vector<QLabelItem*>& labelItems() const { return _labelItems; }

    void setEnabledAllLabelButtons(bool enabled);

    const QLabelItem* findActiveLabelItem() const;
    const QLabelItem* findLabelItemFromName(const QString &name) const;

public slots:
    void on__radioButtonNone_toggled(bool checked);
    void on__radioButtonHorizontal_toggled(bool checked);
    void on__radioButtonVertical_toggled(bool checked);
    void on__radioButtonLabelling_toggled(bool checked);

    void updateAlphaValue(const int alpha);

protected:
    Ui::QLabellingWidget *ui;
    QLabellingView* _view;
    QString _stylesheetString;
    std::vector<QLabelItem*> _labelItems;
    QButtonGroup *_buttonGroup;
};

#endif // QLABELLINGWIDGET_HPP
