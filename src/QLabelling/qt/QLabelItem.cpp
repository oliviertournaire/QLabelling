#include "QLabelItem.hpp"

#include <QDebug>
#include <QColorDialog>
#include <QRadioButton>
#include <QToolButton>

QLabelItem::QLabelItem(QString labelName, QColor labelColor, QWidget *parent) :
    QWidget(parent),
    _labelName(labelName),
    _radioButtonlabel(new QRadioButton(parent)),
    _toolButtonLabelColor(new QToolButton(parent)),
    _labelColor(labelColor)
{
    _radioButtonlabel->setText(_labelName);
    _toolButtonLabelColor->setIconSize( QSize(QLABELLING_TOOLBUTTON_ICON_SIZE,QLABELLING_TOOLBUTTON_ICON_SIZE) );
    _toolButtonLabelColor->setStyleSheet( QString(QLABELLING_TOOLBUTTON_STYLESHEET_STRING).arg(_labelColor.name()) );

    connectAll();
}

QLabelItem::~QLabelItem()
{
    disconnectAll();
    delete _radioButtonlabel;
    delete _toolButtonLabelColor;
}

void QLabelItem::connectAll()
{
    connect(_toolButtonLabelColor,
            SIGNAL(clicked(bool)),
            this,
            SLOT(toolButtonLabelColor_clicked(bool)));
}

void QLabelItem::disconnectAll()
{
    disconnect(_toolButtonLabelColor,
               SIGNAL(clicked(bool)),
               this,
               SLOT(toolButtonLabelColor_clicked(bool)));
}

void QLabelItem::toolButtonLabelColor_clicked(bool checked)
{
    QColor choosedColor = QColorDialog::getColor(_labelColor);
    if (choosedColor.isValid())
    {
        setLabelColor(choosedColor);
        _toolButtonLabelColor->setStyleSheet( QString(QLABELLING_TOOLBUTTON_STYLESHEET_STRING).arg(_labelColor.name()) );
    }
}

QToolButton *QLabelItem::toolButtonLabelColor() const
{
    return _toolButtonLabelColor;
}

void QLabelItem::setToolButtonLabelColor(QToolButton *toolButtonLabelColor)
{
    _toolButtonLabelColor = toolButtonLabelColor;
}

QRadioButton *QLabelItem::radioButtonlabel() const
{
    return _radioButtonlabel;
}

void QLabelItem::setRadioButtonlabel(QRadioButton *radioButtonlabel)
{
    _radioButtonlabel = radioButtonlabel;
}

QString QLabelItem::labelName() const
{
    return _labelName;
}

void QLabelItem::setLabelName(const QString &labelName)
{
    _labelName = labelName;
}

QColor QLabelItem::labelColor() const
{
    return _labelColor;
}

void QLabelItem::setLabelColor( const QColor &labelColor, const int alpha )
{
    _labelColor = labelColor;
    _labelColor.setAlpha(alpha);
}
