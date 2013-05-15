#ifndef QLABELLINGWIDGET_HPP
#define QLABELLINGWIDGET_HPP

#include <QWidget>

#include <vector>

class QLabellingView;
class QGraphicsView;
class QLabelItem;

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

public slots:
    void on__radioButtonHorizontal_toggled(bool checked);
    void on__radioButtonVertical_toggled(bool checked);
    void on__radioButtonLabelling_toggled(bool checked);

protected:
    Ui::QLabellingWidget *ui;
    QLabellingView* _view;
    QString _stylesheetString;
    std::vector<QLabelItem*> _labelItems;
};

#endif // QLABELLINGWIDGET_HPP
