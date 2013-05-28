/********************************************************************************
** Form generated from reading UI file 'OverlayDialog.ui'
**
** Created: Mon May 27 19:06:02 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OVERLAYDIALOG_H
#define UI_OVERLAYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_OverlayDialog
{
public:
    QDialogButtonBox *buttonBox;
    QListWidget *arrangementsListWidget;
    QListWidget *overlayListWidget;
    QPushButton *pickPushButton;
    QPushButton *unpickPushButton;

    void setupUi(QDialog *OverlayDialog)
    {
        if (OverlayDialog->objectName().isEmpty())
            OverlayDialog->setObjectName(QString::fromUtf8("OverlayDialog"));
        OverlayDialog->resize(824, 300);
        buttonBox = new QDialogButtonBox(OverlayDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(450, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        arrangementsListWidget = new QListWidget(OverlayDialog);
        arrangementsListWidget->setObjectName(QString::fromUtf8("arrangementsListWidget"));
        arrangementsListWidget->setGeometry(QRect(20, 20, 256, 192));
        overlayListWidget = new QListWidget(OverlayDialog);
        overlayListWidget->setObjectName(QString::fromUtf8("overlayListWidget"));
        overlayListWidget->setGeometry(QRect(540, 20, 256, 192));
        pickPushButton = new QPushButton(OverlayDialog);
        pickPushButton->setObjectName(QString::fromUtf8("pickPushButton"));
        pickPushButton->setGeometry(QRect(350, 90, 98, 27));
        unpickPushButton = new QPushButton(OverlayDialog);
        unpickPushButton->setObjectName(QString::fromUtf8("unpickPushButton"));
        unpickPushButton->setGeometry(QRect(350, 120, 98, 27));

        retranslateUi(OverlayDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), OverlayDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), OverlayDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(OverlayDialog);
    } // setupUi

    void retranslateUi(QDialog *OverlayDialog)
    {
        OverlayDialog->setWindowTitle(QApplication::translate("OverlayDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        pickPushButton->setText(QApplication::translate("OverlayDialog", ">>", 0, QApplication::UnicodeUTF8));
        unpickPushButton->setText(QApplication::translate("OverlayDialog", "<<", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OverlayDialog: public Ui_OverlayDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OVERLAYDIALOG_H
