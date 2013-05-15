#include "QLabellingWidget.hpp"
#include "ui_QLabellingWidget.h"

#include <QSettings>
#include <QColorDialog>
#include <QPushButton>

#include "QLabellingView.hpp"
#include "config.hpp"

#include "QLabelItem.hpp"

using namespace std;

QLabellingWidget::QLabellingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QLabellingWidget),
    _view(new QLabellingView),
    _stylesheetString(""),
    _labelItems( vector<QLabelItem*>() )
{
    ui->setupUi(this);

    _stylesheetString = QString(QLABELLING_TOOLBUTTON_STYLESHEET_STRING);

    /*
    QLabelItem *item = new QLabelItem("Long item name", Qt::red);
    ui->_gridLayoutLabels->addWidget(item->radioButtonlabel());
    ui->_gridLayoutLabels->addWidget(item->toolButtonLabelColor());
    */

    setWindowIcon( QIcon(":/QLabelling/QLabellingIcon.png") );
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
    _view->setMode( QLabellingView::EDIT_MODE_LABELLING );
    _view->setLabellingMode(true);

    // Task #2: begins here!!!
    /*
    ui->_radioButtonLabellingWindow-> setEnabled(checked);
    ui->_radioButtonLabellingWall->   setEnabled(checked);
    ui->_radioButtonLabellingBalcony->setEnabled(checked);
    ui->_radioButtonLabellingDoor->   setEnabled(checked);
    ui->_radioButtonLabellingShop->   setEnabled(checked);
    ui->_radioButtonLabellingRoof->   setEnabled(checked);
    ui->_radioButtonLabellingSky->    setEnabled(checked);
    ui->_radioButtonLabellingUnknow-> setEnabled(checked);
    */
    // Task #2: ends here!!!
}