#include "QLabellingWidget.hpp"
#include "ui_QLabellingWidget.h"

#include <QSettings>
#include <QColorDialog>
#include <QToolButton>
#include <QButtonGroup>

#include "QLabellingView.hpp"
#include "config.hpp"

#include "QLabelItem.hpp"

using namespace std;

QLabellingWidget::QLabellingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QLabellingWidget),
    _view(new QLabellingView),
    _stylesheetString(""),
    _labelItems( vector<QLabelItem*>() ),
    _buttonGroup( new QButtonGroup(parent) )
{
    ui->setupUi(this);

    _stylesheetString = QString(QLABELLING_TOOLBUTTON_STYLESHEET_STRING);

    QLabelItem *wallItem = new QLabelItem("Wall", Qt::red);
    ui->_gridLayoutLabels->addWidget(wallItem->radioButtonlabel(), 0, 0);
    ui->_gridLayoutLabels->addWidget(wallItem->toolButtonLabelColor(), 0, 1);
    _labelItems.push_back(wallItem);
    _buttonGroup->addButton(wallItem->radioButtonlabel());

    QLabelItem *windowItem = new QLabelItem("Window", Qt::yellow);
    ui->_gridLayoutLabels->addWidget(windowItem->radioButtonlabel(), 0, 2);
    ui->_gridLayoutLabels->addWidget(windowItem->toolButtonLabelColor(), 0, 3);
    _labelItems.push_back(windowItem);
    _buttonGroup->addButton(windowItem->radioButtonlabel());

    setEnabledAllLabelButtons(false);

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

void QLabellingWidget::setEnabledAllLabelButtons( bool enabled )
{
    for(unsigned int i=0;i<_labelItems.size();++i)
        _labelItems[i]->radioButtonlabel()->setEnabled(enabled);
}

void QLabellingWidget::on__radioButtonNone_toggled(bool)
{
    _view->setMode( QLabellingView::EDIT_MODE_NONE );
    _view->setLabellingMode(false);
    setEnabledAllLabelButtons(false);
}

void QLabellingWidget::on__radioButtonHorizontal_toggled(bool)
{
    _view->setMode( QLabellingView::EDIT_MODE_HORIZONTAL_LINE );
    _view->setLabellingMode(false);
    setEnabledAllLabelButtons(false);
}

void QLabellingWidget::on__radioButtonVertical_toggled(bool)
{
    _view->setMode( QLabellingView::EDIT_MODE_VERTICAL_LINE );
    _view->setLabellingMode(false);
    setEnabledAllLabelButtons(false);
}

void QLabellingWidget::on__radioButtonLabelling_toggled(bool checked)
{
    _view->setMode( QLabellingView::EDIT_MODE_LABELLING );
    _view->setLabellingMode(true);
    setEnabledAllLabelButtons(true);
}