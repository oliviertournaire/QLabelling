#include "QLabellingWidget.hpp"
#include "ui_QLabellingWidget.h"

#include <QSettings>
#include <QColorDialog>

#include "QLabellingView.hpp"

QLabellingWidget::QLabellingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QLabellingWidget),
    _view(new QLabellingView)
{
    ui->setupUi(this);

    _stylesheetString = QString("QToolButton{ background: %1; border: 2px solid #8f8f91; border-radius: 6px;}");

    QSettings settings("IMAGINE", "QSnapEdit");

    settings.beginGroup("QSnapEditView");

    qreal lineWidth = settings.value("lineWidth", 1.).toReal();
    _view->setLineWidth(lineWidth);
    ui->_doubleSpinBoxLineWidth->setValue(lineWidth);

    QColor lineColor = _view->lineColor();
    ui->_toolButtonLineColor->setStyleSheet(_stylesheetString.arg(lineColor.name()));

    Qt::PenStyle lineStyle = _view->lineStyle();
    ui->_comboBoxLineStyle->setCurrentIndex((int)lineStyle);

    QColor windowColor = _view->windowColor();
    ui->_toolButtonWindowColor->setStyleSheet(_stylesheetString.arg(windowColor.name()));

    QColor wallColor = _view->wallColor();
    ui->_toolButtonWallColor->setStyleSheet(_stylesheetString.arg(wallColor.name()));

    QColor balconyColor = _view->balconyColor();
    ui->_toolButtonBalconyColor->setStyleSheet(_stylesheetString.arg(balconyColor.name()));

    QColor doorColor = _view->doorColor();
    ui->_toolButtonDoorColor->setStyleSheet(_stylesheetString.arg(doorColor.name()));

    QColor shopColor = _view->shopColor();
    ui->_toolButtonShopColor->setStyleSheet(_stylesheetString.arg(shopColor.name()));

    QColor roofColor = _view->roofColor();
    ui->_toolButtonRoofColor->setStyleSheet(_stylesheetString.arg(roofColor.name()));

    QColor skyColor = _view->skyColor();
    ui->_toolButtonSkyColor->setStyleSheet(_stylesheetString.arg(skyColor.name()));

    QColor unknowColor = _view->unknowColor();
    ui->_toolButtonUnknowColor->setStyleSheet(_stylesheetString.arg(unknowColor.name()));

    settings.endGroup();

    setWindowIcon( QIcon(":/QSnapEdit/QSnapEditIcon.png") );
}

QLabellingWidget::~QLabellingWidget()
{
    delete ui;
}

QLabellingView* QLabellingWidget::view()
{
    return _view;
}

QImage QLabellingWidget::labelsImage()
{
    return _view->labelsImage();
}

void QLabellingWidget::on__radioButtonHorizontal_toggled(bool)
{
    _view->setMode( QLabellingView::EDIT_MODE_HORIZONTAL_LINE );
    _view->setLabellingMode(false);
}

void QLabellingWidget::on__radioButtonVertical_toggled(bool)
{
    _view->setMode( QLabellingView::EDIT_MODE_VERTICAL_LINE );
    _view->setLabellingMode(false);
}

void QLabellingWidget::on__radioButtonLabelling_toggled(bool checked)
{
    _view->setLabellingMode(true);

    ui->_radioButtonLabellingWindow-> setEnabled(checked);
    ui->_radioButtonLabellingWall->   setEnabled(checked);
    ui->_radioButtonLabellingBalcony->setEnabled(checked);
    ui->_radioButtonLabellingDoor->   setEnabled(checked);
    ui->_radioButtonLabellingShop->   setEnabled(checked);
    ui->_radioButtonLabellingRoof->   setEnabled(checked);
    ui->_radioButtonLabellingSky->    setEnabled(checked);
    ui->_radioButtonLabellingUnknow-> setEnabled(checked);

    if (ui->_radioButtonLabellingWindow->isChecked())
        _view->setMode( QLabellingView::EDIT_MODE_LABELLING_WINDOW );
    else if (ui->_radioButtonLabellingWall->isChecked())
        _view->setMode( QLabellingView::EDIT_MODE_LABELLING_WALL );
    else if (ui->_radioButtonLabellingBalcony->isChecked())
        _view->setMode( QLabellingView::EDIT_MODE_LABELLING_BALCONY );
    else if (ui->_radioButtonLabellingDoor->isChecked())
        _view->setMode( QLabellingView::EDIT_MODE_LABELLING_DOOR );
    else if (ui->_radioButtonLabellingShop->isChecked())
        _view->setMode( QLabellingView::EDIT_MODE_LABELLING_SHOP );
    else if (ui->_radioButtonLabellingRoof->isChecked())
        _view->setMode( QLabellingView::EDIT_MODE_LABELLING_ROOF );
    else if (ui->_radioButtonLabellingSky->isChecked())
        _view->setMode( QLabellingView::EDIT_MODE_LABELLING_SKY );
    else
        _view->setMode( QLabellingView::EDIT_MODE_LABELLING_UNKNOW );
}

void QLabellingWidget::on__radioButtonLabellingWindow_toggled(bool checked)
{
    _view->setMode(QLabellingView::EDIT_MODE_LABELLING_WINDOW);
}

void QLabellingWidget::on__radioButtonLabellingWall_toggled(bool checked)
{
    _view->setMode(QLabellingView::EDIT_MODE_LABELLING_WALL);
}

void QLabellingWidget::on__radioButtonLabellingBalcony_toggled(bool checked)
{
    _view->setMode(QLabellingView::EDIT_MODE_LABELLING_BALCONY);
}
void QLabellingWidget::on__radioButtonLabellingDoor_toggled(bool checked)
{
    _view->setMode(QLabellingView::EDIT_MODE_LABELLING_DOOR);
}

void QLabellingWidget::on__radioButtonLabellingShop_toggled(bool checked)
{
    _view->setMode(QLabellingView::EDIT_MODE_LABELLING_SHOP);
}

void QLabellingWidget::on__radioButtonLabellingRoof_toggled(bool checked)
{
    _view->setMode(QLabellingView::EDIT_MODE_LABELLING_ROOF);
}

void QLabellingWidget::on__radioButtonLabellingSky_toggled(bool checked)
{
    _view->setMode(QLabellingView::EDIT_MODE_LABELLING_SKY);
}

void QLabellingWidget::on__radioButtonLabellingUnknow_toggled(bool checked)
{
    _view->setMode(QLabellingView::EDIT_MODE_LABELLING_UNKNOW);
}

void QLabellingWidget::on__doubleSpinBoxLineWidth_valueChanged(double d)
{
    _view->setLineWidth(qreal(d));
}

void QLabellingWidget::on__comboBoxLineStyle_currentIndexChanged(int index)
{
    _view->setLineStyle( (Qt::PenStyle)index );
}

void QLabellingWidget::on__toolButtonLineColor_clicked(bool)
{
    QColor lineColor = QColorDialog::getColor(_view->lineColor());
    if (lineColor.isValid())
        _view->setLineColor(lineColor);
    ui->_toolButtonLineColor->setStyleSheet(_stylesheetString.arg(lineColor.name()));
}

void QLabellingWidget::on__toolButtonWindowColor_clicked(bool)
{
    QColor windowColor = QColorDialog::getColor(_view->windowColor());
    if (windowColor.isValid())
        _view->setWindowColor(windowColor);
    ui->_toolButtonWindowColor->setStyleSheet(_stylesheetString.arg(windowColor.name()));
}

void QLabellingWidget::on__toolButtonWallColor_clicked(bool)
{
    QColor wallColor = QColorDialog::getColor(_view->wallColor());
    if (wallColor.isValid())
        _view->setWallColor(wallColor);
    ui->_toolButtonWallColor->setStyleSheet(_stylesheetString.arg(wallColor.name()));
}

void QLabellingWidget::on__toolButtonBalconyColor_clicked(bool)
{
    QColor balconyColor = QColorDialog::getColor(_view->balconyColor());
    if (balconyColor.isValid())
        _view->setBalconyColor(balconyColor);
    ui->_toolButtonBalconyColor->setStyleSheet(_stylesheetString.arg(balconyColor.name()));
}

void QLabellingWidget::on__toolButtonDoorColor_clicked(bool)
{
    QColor doorColor = QColorDialog::getColor(_view->doorColor());
    if (doorColor.isValid())
        _view->setDoorColor(doorColor);
    ui->_toolButtonDoorColor->setStyleSheet(_stylesheetString.arg(doorColor.name()));
}

void QLabellingWidget::on__toolButtonShopColor_clicked(bool)
{
    QColor shopColor = QColorDialog::getColor(_view->shopColor());
    if (shopColor.isValid())
        _view->setShopColor(shopColor);
    ui->_toolButtonShopColor->setStyleSheet(_stylesheetString.arg(shopColor.name()));
}

void QLabellingWidget::on__toolButtonRoofColor_clicked(bool)
{
    QColor roofColor = QColorDialog::getColor(_view->roofColor());
    if (roofColor.isValid())
        _view->setRoofColor(roofColor);
    ui->_toolButtonRoofColor->setStyleSheet(_stylesheetString.arg(roofColor.name()));
}

void QLabellingWidget::on__toolButtonSkyColor_clicked(bool)
{
    QColor skyColor = QColorDialog::getColor(_view->skyColor());
    if (skyColor.isValid())
        _view->setSkyColor(skyColor);
    ui->_toolButtonSkyColor->setStyleSheet(_stylesheetString.arg(skyColor.name()));
}

void QLabellingWidget::on__toolButtonUnknowColor_clicked(bool)
{
    QColor unknowColor = QColorDialog::getColor(_view->unknowColor());
    if (unknowColor.isValid())
        _view->setUnknowColor(unknowColor);
    ui->_toolButtonUnknowColor->setStyleSheet(_stylesheetString.arg(unknowColor.name()));
}

void QLabellingWidget::on__sliderAlpha_sliderReleased()
{
    _view->setAlpha( ui->_sliderAlpha->value() );
}
