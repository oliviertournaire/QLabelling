#include "QArrangementLabellingWidget.h"
#include "ui_QArrangementLabellingWidget.h"

#include <QSettings>
#include <QColorDialog>
#include <QToolButton>
#include <QButtonGroup>
#include <QFileDialog>

#include <fstream>

#include "config.hpp"

#include "QLabelItem.hpp"
#include "QArrangementLabellingLogWidget.h"

using namespace std;

QArrangementLabellingWidget::QArrangementLabellingWidget() :
QWidget(),
    ui(new Ui::QArrangementLabellingWidget),
    _stylesheetString(""),
    _buttonGroup( new QButtonGroup() ),
    _labelsPath("")
{
    ui->setupUi(this);

    _stylesheetString = QString(QLABELLING_TOOLBUTTON_STYLESHEET_STRING);

    setEnabledAllLabelButtons(false);
    setWindowIcon( QIcon(":/QLabelling/QLabellingIcon.png") );

//     QLabellingView::instance()->readSettings();
}

QArrangementLabellingWidget::~QArrangementLabellingWidget()
{
    delete ui;
}

void QArrangementLabellingWidget::setEnabledAllLabelButtons( bool enabled )
{
    for(unsigned int i=0;i<_labelItems.size();++i)
        _labelItems[i]->radioButtonlabel()->setEnabled(enabled);
    
    if(enabled && _labelItems.size()>0)
        _labelItems[0]->radioButtonlabel()->setChecked(true);
}

void QArrangementLabellingWidget::parseLabels()
{
    // Then, parse file
    ifstream ifs(_labelsPath.toStdString().c_str());
    bool foundUnknowLabel = false;
    string labelname;
    int r, g, b, a;
    int row = 0, col = 0;
    if(ifs.good())
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
            connect(item->radioButtonlabel(), SIGNAL(clicked(bool)),
                this, SIGNAL(labelChanged()));
            ui->_gridLayoutLabels->addWidget(item->radioButtonlabel(), row, col);
            ui->_gridLayoutLabels->addWidget(item->toolButtonLabelColor(), row, col+1);
            _labelItems.push_back(item);
            _buttonGroup->addButton(item->radioButtonlabel());
            ++cpt;
            col += 2;
            // Check if mandatory QLABELLING_UNKNOW_LABEL_STRING label has been found
            if(!foundUnknowLabel)
                if(QString::fromStdString(labelname).toLower() == QLABELLING_UNKNOW_LABEL_STRING)
                    foundUnknowLabel = true;
        }
    }
    else
    {
        ui->_lineEditLabelsPath->setText( tr("Labels path") + _labelsPath + tr(" not found!") );
        ifs.close();
        return;
    }

    if(!foundUnknowLabel)
    {
        QLabelItem *item = new QLabelItem(QLABELLING_UNKNOW_LABEL_STRING, QLABELLING_UNKNOW_LABEL_BRUSH_COLOR);
        connect(item->radioButtonlabel(), SIGNAL(clicked(bool)),
                this, SIGNAL(labelChanged()));
        ui->_gridLayoutLabels->addWidget(item->radioButtonlabel(), row, col);
        ui->_gridLayoutLabels->addWidget(item->toolButtonLabelColor(), row, col+1);
        _labelItems.push_back(item);
        _buttonGroup->addButton(item->radioButtonlabel());
    }

    ifs.close();
}

void QArrangementLabellingWidget::setLabelsPath(const QString &labelsPath)
{
    _labelsPath = labelsPath;
    parseLabels();
}

const QLabelItem* QArrangementLabellingWidget::findActiveLabelItem() const
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

const QLabelItem* QArrangementLabellingWidget::findLabelItemFromName(const QString &name) const
{
    for(unsigned int i=0;i<_labelItems.size();++i)
        if ( _labelItems[i]->labelName() == name )
            return _labelItems[i];
    return NULL;
}

const QString QArrangementLabellingWidget::Color2Label(const QColor color) const
{
    for(unsigned int i=0;i<_labelItems.size();++i)
        if ( _labelItems[i]->labelColor() == color )
            return _labelItems[i]->labelName();
    return QLABELLING_UNKNOW_LABEL_STRING;
}

void QArrangementLabellingWidget::on__toolButtonChooseLabelsPath_clicked(bool checked)
{
    _labelsPath = QFileDialog::getOpenFileName(this, tr("Open label file"), "", tr("Label Files (*.txt);;All files (*.*)"));
    if(!_labelsPath.isNull())
        parseLabels();
}

void QArrangementLabellingWidget::updateAlphaValue(const int alpha)
{
    for(unsigned int i=0;i<_labelItems.size();++i)
    {
        QColor labelItemColor = _labelItems[i]->labelColor();
        labelItemColor.setAlpha(alpha);
        _labelItems[i]->setLabelColor(labelItemColor);
    }
}