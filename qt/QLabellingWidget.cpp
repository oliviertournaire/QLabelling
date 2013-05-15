#include "QLabellingWidget.hpp"
#include "ui_QLabellingWidget.h"

#include <QSettings>
#include <QColorDialog>
#include <QToolButton>
#include <QButtonGroup>
#include <QFileDialog>

#include <fstream>

#include "QLabellingView.hpp"
#include "config.hpp"

#include "QLabelItem.hpp"

using namespace std;

QLabellingWidget::QLabellingWidget( const QString &labelsPath, QWidget *parent /*= 0*/ ) :
QWidget(parent),
    ui(new Ui::QLabellingWidget),
    _stylesheetString(""),
    _buttonGroup( new QButtonGroup(parent) ),
    _labelsPath(labelsPath)
{
    ui->setupUi(this);

    parseLabels();

    _stylesheetString = QString(QLABELLING_TOOLBUTTON_STYLESHEET_STRING);

    setEnabledAllLabelButtons(false);
    setWindowIcon( QIcon(":/QLabelling/QLabellingIcon.png") );

    // The view needs to be constructed after all initialization
    _view = new QLabellingView;
    _view->setLabellingWidget(this);
    _view->readSettings();
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

void QLabellingWidget::parseLabels()
{
    ui->_lineEditLabelsPath->setText(_labelsPath);
    // First, empty existing labels
    for(unsigned int i=0;i<_labelItems.size();++i)
    {
        ui->_gridLayoutLabels->removeWidget(_labelItems[i]->radioButtonlabel());
        ui->_gridLayoutLabels->removeWidget(_labelItems[i]->toolButtonLabelColor());
    }
    QList<QAbstractButton *> allButtons = _buttonGroup->buttons();
    for (int i=0;i<allButtons.size();++i)
        _buttonGroup->removeButton(allButtons[i]);
    for(unsigned int i=0;i<_labelItems.size();++i)
        delete _labelItems[i];
    _labelItems.erase(_labelItems.begin(), _labelItems.end());

    // Then, parse file
    ifstream ifs(_labelsPath.toStdString().c_str());
    string labelname;
    int r, g, b, a;
    if(ifs.good())
    {
        int row = 0, col = 0;
        int cpt = 0;
        while(!ifs.eof())
        {
            if (cpt%2 == 0)
            {
                ++row;
                col = 0;
            }
            ifs >> labelname >> r >> g >> b >> a;
            QColor labelcolor(r,g,b,a);
            QLabelItem *item = new QLabelItem(QString::fromStdString(labelname), labelcolor);
            ui->_gridLayoutLabels->addWidget(item->radioButtonlabel(), row, col);
            ui->_gridLayoutLabels->addWidget(item->toolButtonLabelColor(), row, col+1);
            _labelItems.push_back(item);
            _buttonGroup->addButton(item->radioButtonlabel());
            ++cpt;
            col += 2;
        }
    }
}

void QLabellingWidget::setLabelsPath(const QString &labelsPath)
{
    _labelsPath = labelsPath;
    parseLabels();
}

const QLabelItem* QLabellingWidget::findActiveLabelItem() const
{
    QAbstractButton *selectedButton = _buttonGroup->checkedButton();
    QRadioButton *selectedRadioButton = dynamic_cast<QRadioButton*>(selectedButton);
    if (!selectedRadioButton)
        return NULL;
    for(unsigned int i=0;i<_labelItems.size();++i)
        if ( _labelItems[i]->radioButtonlabel() == selectedRadioButton )
            return _labelItems[i];
    return NULL;
}

const QLabelItem* QLabellingWidget::findLabelItemFromName(const QString &name) const
{
    for(unsigned int i=0;i<_labelItems.size();++i)
        if ( _labelItems[i]->labelName() == name )
            return _labelItems[i];
    return NULL;
}

void QLabellingWidget::on__toolButtonChooseLabelsPath_clicked(bool checked)
{
    _labelsPath = QFileDialog::getOpenFileName(this, tr("Open label file"), "", tr("Label Files (*.txt)"));
    parseLabels();
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

void QLabellingWidget::updateAlphaValue(const int alpha)
{
    for(unsigned int i=0;i<_labelItems.size();++i)
    {
        QColor labelItemColor = _labelItems[i]->labelColor();
        labelItemColor.setAlpha(alpha);
        _labelItems[i]->setLabelColor(labelItemColor);
    }
}