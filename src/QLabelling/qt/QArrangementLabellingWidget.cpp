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
#include <QInputDialog>
#include <QLabel>

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
    QObject::connect(this->ui->addLabel,SIGNAL(clicked()),this,SLOT(addLabelChoice()));

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

const QColor QArrangementLabellingWidget::Label2Color(const QString label) const
{
    for(unsigned int i=0;i<_labelItems.size();++i)
        if ( _labelItems[i]->labelName() == label )
            return _labelItems[i]->labelColor();
    return QLABELLING_UNKNOW_LABEL_BRUSH_COLOR;
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
void QArrangementLabellingWidget::addLabelPlus(QString labelname, QColor labelcolor){
     QLabelItem *item = new QLabelItem(labelname, labelcolor);
     item->setLabelColor(labelcolor,127);

     connect(item->radioButtonlabel(), SIGNAL(clicked(bool)),
         this, SIGNAL(labelChanged()));
     int row=_labelItems.size()/2+1;
     int col=((_labelItems.size()+1)/2-_labelItems.size()/2)*2;
     ui->_gridLayoutLabels->addWidget(item->radioButtonlabel(), row, col);
     ui->_gridLayoutLabels->addWidget(item->toolButtonLabelColor(), row, col+1);
     _labelItems.push_back(item);
     _buttonGroup->addButton(item->radioButtonlabel());
     if(!_labelItems[0]->radioButtonlabel()->isEnabled())
      setEnabledAllLabelButtons(false);
}
void QArrangementLabellingWidget::addLabelChoice(){
    QString texttoLabel;
    bool ok;
       QString text = QInputDialog::getText(this, tr("New Label"),
                                            tr("Name your label"), QLineEdit::Normal,
                                            tr("label") + QString::number(_labelItems.size()+1), &ok);
       if (ok && !text.isEmpty())
           texttoLabel=text;
       QColor defaultcolor(50,50,50,127);
       QColor choosedColor = QColorDialog::getColor(defaultcolor);
       if (! choosedColor.isValid())
       {
          return;
       }
       addLabelPlus(texttoLabel,choosedColor);

}
