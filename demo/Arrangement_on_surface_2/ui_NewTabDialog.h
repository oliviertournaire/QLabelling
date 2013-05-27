/********************************************************************************
** Form generated from reading UI file 'NewTabDialog.ui'
**
** Created: Mon May 27 19:06:01 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWTABDIALOG_H
#define UI_NEWTABDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewTabDialog
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QRadioButton *segmentRadioButton;
    QRadioButton *polylineRadioButton;
    QRadioButton *conicRadioButton;
    QDialogButtonBox *buttonBox;
    QRadioButton *linearRadioButton;
    QRadioButton *circularArcRadioButton;

    void setupUi(QDialog *NewTabDialog)
    {
        if (NewTabDialog->objectName().isEmpty())
            NewTabDialog->setObjectName(QString::fromUtf8("NewTabDialog"));
        NewTabDialog->resize(400, 215);
        gridLayoutWidget = new QWidget(NewTabDialog);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 381, 197));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        segmentRadioButton = new QRadioButton(gridLayoutWidget);
        segmentRadioButton->setObjectName(QString::fromUtf8("segmentRadioButton"));
        segmentRadioButton->setChecked(true);

        gridLayout->addWidget(segmentRadioButton, 0, 0, 1, 1);

        polylineRadioButton = new QRadioButton(gridLayoutWidget);
        polylineRadioButton->setObjectName(QString::fromUtf8("polylineRadioButton"));
        polylineRadioButton->setEnabled(true);

        gridLayout->addWidget(polylineRadioButton, 1, 0, 1, 1);

        conicRadioButton = new QRadioButton(gridLayoutWidget);
        conicRadioButton->setObjectName(QString::fromUtf8("conicRadioButton"));

        gridLayout->addWidget(conicRadioButton, 2, 0, 1, 1);

        buttonBox = new QDialogButtonBox(gridLayoutWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 6, 0, 1, 1);

        linearRadioButton = new QRadioButton(gridLayoutWidget);
        linearRadioButton->setObjectName(QString::fromUtf8("linearRadioButton"));

        gridLayout->addWidget(linearRadioButton, 3, 0, 1, 1);

        circularArcRadioButton = new QRadioButton(gridLayoutWidget);
        circularArcRadioButton->setObjectName(QString::fromUtf8("circularArcRadioButton"));

        gridLayout->addWidget(circularArcRadioButton, 4, 0, 1, 1);


        retranslateUi(NewTabDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), NewTabDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), NewTabDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(NewTabDialog);
    } // setupUi

    void retranslateUi(QDialog *NewTabDialog)
    {
        NewTabDialog->setWindowTitle(QApplication::translate("NewTabDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        segmentRadioButton->setText(QApplication::translate("NewTabDialog", "Segment", 0, QApplication::UnicodeUTF8));
        polylineRadioButton->setText(QApplication::translate("NewTabDialog", "Polyline", 0, QApplication::UnicodeUTF8));
        conicRadioButton->setText(QApplication::translate("NewTabDialog", "Conic", 0, QApplication::UnicodeUTF8));
        linearRadioButton->setText(QApplication::translate("NewTabDialog", "Linear", 0, QApplication::UnicodeUTF8));
        circularArcRadioButton->setText(QApplication::translate("NewTabDialog", "Circular Arc", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NewTabDialog: public Ui_NewTabDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWTABDIALOG_H
