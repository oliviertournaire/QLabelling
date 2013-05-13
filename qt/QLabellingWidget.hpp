#ifndef QLABELLINGWIDGET_HPP
#define QLABELLINGWIDGET_HPP

#include <QWidget>

class QLabellingView;
class QGraphicsView;

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
    void on__radioButtonLabellingWindow_toggled(bool checked);
    void on__radioButtonLabellingWall_toggled(bool checked);
    void on__radioButtonLabellingBalcony_toggled(bool checked);
    void on__radioButtonLabellingDoor_toggled(bool checked);
    void on__radioButtonLabellingShop_toggled(bool checked);
    void on__radioButtonLabellingRoof_toggled(bool checked);
    void on__radioButtonLabellingSky_toggled(bool checked);
    void on__radioButtonLabellingUnknow_toggled(bool checked);
    void on__toolButtonWindowColor_clicked(bool checked = false);
    void on__toolButtonWallColor_clicked(bool checked = false);
    void on__toolButtonBalconyColor_clicked(bool checked = false);
    void on__toolButtonDoorColor_clicked(bool checked = false);
    void on__toolButtonShopColor_clicked(bool checked = false);
    void on__toolButtonRoofColor_clicked(bool checked = false);
    void on__toolButtonSkyColor_clicked(bool checked = false);
    void on__toolButtonUnknowColor_clicked(bool checked = false);

protected:
    Ui::QLabellingWidget *ui;
    QLabellingView* _view;
    QString _stylesheetString;
};

#endif // QLABELLINGWIDGET_HPP
