/********************************************************************************
** Form generated from reading UI file 'ArrangementDemoWindow.ui'
**
** Created: Mon May 27 19:06:01 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARRANGEMENTDEMOWINDOW_H
#define UI_ARRANGEMENTDEMOWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ArrangementDemoWindow
{
public:
    QAction *actionQuit;
    QAction *actionInsert;
    QAction *actionDelete;
    QAction *actionDrag;
    QAction *actionPointLocation;
    QAction *actionRayShootingUp;
    QAction *actionRayShootingDown;
    QAction *actionMerge;
    QAction *actionSplit;
    QAction *actionFill;
    QAction *actionSnapMode;
    QAction *actionGridSnapMode;
    QAction *actionSegment;
    QAction *actionPolyline;
    QAction *actionConic;
    QAction *actionNewTab;
    QAction *actionOverlay;
    QAction *actionPreferences;
    QAction *actionShowGrid;
    QAction *actionLowerEnvelope;
    QAction *actionUpperEnvelope;
    QAction *actionConicSegment;
    QAction *actionConicCircle;
    QAction *actionConicEllipse;
    QAction *actionConicThreePoint;
    QAction *actionConicFivePoint;
    QAction *actionCloseTab;
    QAction *actionSaveAs;
    QAction *actionOpen;
    QAction *actionPrintConicCurves;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionCurveRay;
    QAction *actionCurveLine;
    QAction *actionFillColor;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menuMode;
    QMenu *menu_Edit;
    QMenu *menuT_ools;

    void setupUi(QMainWindow *ArrangementDemoWindow)
    {
        if (ArrangementDemoWindow->objectName().isEmpty())
            ArrangementDemoWindow->setObjectName(QString::fromUtf8("ArrangementDemoWindow"));
        ArrangementDemoWindow->resize(946, 600);
        actionQuit = new QAction(ArrangementDemoWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionInsert = new QAction(ArrangementDemoWindow);
        actionInsert->setObjectName(QString::fromUtf8("actionInsert"));
        actionInsert->setCheckable(true);
        actionInsert->setChecked(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/demo_insert.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionInsert->setIcon(icon);
        actionDelete = new QAction(ArrangementDemoWindow);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        actionDelete->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/demo_delete.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon1);
        actionDrag = new QAction(ArrangementDemoWindow);
        actionDrag->setObjectName(QString::fromUtf8("actionDrag"));
        actionDrag->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/hand.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionDrag->setIcon(icon2);
        actionPointLocation = new QAction(ArrangementDemoWindow);
        actionPointLocation->setObjectName(QString::fromUtf8("actionPointLocation"));
        actionPointLocation->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/demo_pointlocation.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionPointLocation->setIcon(icon3);
        actionRayShootingUp = new QAction(ArrangementDemoWindow);
        actionRayShootingUp->setObjectName(QString::fromUtf8("actionRayShootingUp"));
        actionRayShootingUp->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/demo_rayshoot_up.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionRayShootingUp->setIcon(icon4);
        actionRayShootingDown = new QAction(ArrangementDemoWindow);
        actionRayShootingDown->setObjectName(QString::fromUtf8("actionRayShootingDown"));
        actionRayShootingDown->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/demo_rayshoot_down.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionRayShootingDown->setIcon(icon5);
        actionMerge = new QAction(ArrangementDemoWindow);
        actionMerge->setObjectName(QString::fromUtf8("actionMerge"));
        actionMerge->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/demo_merge.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionMerge->setIcon(icon6);
        actionSplit = new QAction(ArrangementDemoWindow);
        actionSplit->setObjectName(QString::fromUtf8("actionSplit"));
        actionSplit->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/demo_split.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionSplit->setIcon(icon7);
        actionFill = new QAction(ArrangementDemoWindow);
        actionFill->setObjectName(QString::fromUtf8("actionFill"));
        actionFill->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/demo_fill.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionFill->setIcon(icon8);
        actionSnapMode = new QAction(ArrangementDemoWindow);
        actionSnapMode->setObjectName(QString::fromUtf8("actionSnapMode"));
        actionSnapMode->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/demo_snapvertex.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionSnapMode->setIcon(icon9);
        actionGridSnapMode = new QAction(ArrangementDemoWindow);
        actionGridSnapMode->setObjectName(QString::fromUtf8("actionGridSnapMode"));
        actionGridSnapMode->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/demo_snapgrid.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionGridSnapMode->setIcon(icon10);
        actionSegment = new QAction(ArrangementDemoWindow);
        actionSegment->setObjectName(QString::fromUtf8("actionSegment"));
        actionPolyline = new QAction(ArrangementDemoWindow);
        actionPolyline->setObjectName(QString::fromUtf8("actionPolyline"));
        actionConic = new QAction(ArrangementDemoWindow);
        actionConic->setObjectName(QString::fromUtf8("actionConic"));
        actionNewTab = new QAction(ArrangementDemoWindow);
        actionNewTab->setObjectName(QString::fromUtf8("actionNewTab"));
        actionOverlay = new QAction(ArrangementDemoWindow);
        actionOverlay->setObjectName(QString::fromUtf8("actionOverlay"));
        actionPreferences = new QAction(ArrangementDemoWindow);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        actionShowGrid = new QAction(ArrangementDemoWindow);
        actionShowGrid->setObjectName(QString::fromUtf8("actionShowGrid"));
        actionShowGrid->setCheckable(true);
        actionLowerEnvelope = new QAction(ArrangementDemoWindow);
        actionLowerEnvelope->setObjectName(QString::fromUtf8("actionLowerEnvelope"));
        actionLowerEnvelope->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/lower_env_xpm.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionLowerEnvelope->setIcon(icon11);
        actionUpperEnvelope = new QAction(ArrangementDemoWindow);
        actionUpperEnvelope->setObjectName(QString::fromUtf8("actionUpperEnvelope"));
        actionUpperEnvelope->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/upper_env_xpm.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionUpperEnvelope->setIcon(icon12);
        actionConicSegment = new QAction(ArrangementDemoWindow);
        actionConicSegment->setObjectName(QString::fromUtf8("actionConicSegment"));
        actionConicSegment->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/demo_conic_segment.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionConicSegment->setIcon(icon13);
        actionConicCircle = new QAction(ArrangementDemoWindow);
        actionConicCircle->setObjectName(QString::fromUtf8("actionConicCircle"));
        actionConicCircle->setCheckable(true);
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/icons/demo_conic_circle.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionConicCircle->setIcon(icon14);
        actionConicEllipse = new QAction(ArrangementDemoWindow);
        actionConicEllipse->setObjectName(QString::fromUtf8("actionConicEllipse"));
        actionConicEllipse->setCheckable(true);
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icons/demo_conic_ellipse.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionConicEllipse->setIcon(icon15);
        actionConicThreePoint = new QAction(ArrangementDemoWindow);
        actionConicThreePoint->setObjectName(QString::fromUtf8("actionConicThreePoint"));
        actionConicThreePoint->setCheckable(true);
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/icons/demo_conic_3points.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionConicThreePoint->setIcon(icon16);
        actionConicFivePoint = new QAction(ArrangementDemoWindow);
        actionConicFivePoint->setObjectName(QString::fromUtf8("actionConicFivePoint"));
        actionConicFivePoint->setCheckable(true);
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/icons/demo_conic_5points.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionConicFivePoint->setIcon(icon17);
        actionCloseTab = new QAction(ArrangementDemoWindow);
        actionCloseTab->setObjectName(QString::fromUtf8("actionCloseTab"));
        actionSaveAs = new QAction(ArrangementDemoWindow);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        actionOpen = new QAction(ArrangementDemoWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionPrintConicCurves = new QAction(ArrangementDemoWindow);
        actionPrintConicCurves->setObjectName(QString::fromUtf8("actionPrintConicCurves"));
        actionZoomIn = new QAction(ArrangementDemoWindow);
        actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/icons/demo_zoomin.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomIn->setIcon(icon18);
        actionZoomOut = new QAction(ArrangementDemoWindow);
        actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/icons/demo_zoomout.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomOut->setIcon(icon19);
        actionCurveRay = new QAction(ArrangementDemoWindow);
        actionCurveRay->setObjectName(QString::fromUtf8("actionCurveRay"));
        actionCurveRay->setCheckable(true);
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/icons/demo_conic_ray.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionCurveRay->setIcon(icon20);
        actionCurveLine = new QAction(ArrangementDemoWindow);
        actionCurveLine->setObjectName(QString::fromUtf8("actionCurveLine"));
        actionCurveLine->setCheckable(true);
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/icons/demo_conic_line.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        actionCurveLine->setIcon(icon21);
        actionFillColor = new QAction(ArrangementDemoWindow);
        actionFillColor->setObjectName(QString::fromUtf8("actionFillColor"));
        centralwidget = new QWidget(ArrangementDemoWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        ArrangementDemoWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(ArrangementDemoWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ArrangementDemoWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(ArrangementDemoWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        ArrangementDemoWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        menubar = new QMenuBar(ArrangementDemoWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 946, 23));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuMode = new QMenu(menubar);
        menuMode->setObjectName(QString::fromUtf8("menuMode"));
        menu_Edit = new QMenu(menubar);
        menu_Edit->setObjectName(QString::fromUtf8("menu_Edit"));
        menuT_ools = new QMenu(menubar);
        menuT_ools->setObjectName(QString::fromUtf8("menuT_ools"));
        ArrangementDemoWindow->setMenuBar(menubar);

        toolBar->addAction(actionDrag);
        toolBar->addAction(actionInsert);
        toolBar->addAction(actionDelete);
        toolBar->addAction(actionPointLocation);
        toolBar->addAction(actionRayShootingUp);
        toolBar->addAction(actionRayShootingDown);
        toolBar->addAction(actionMerge);
        toolBar->addAction(actionSplit);
        toolBar->addSeparator();
        toolBar->addAction(actionFill);
        toolBar->addAction(actionFillColor);
        toolBar->addSeparator();
        toolBar->addAction(actionZoomIn);
        toolBar->addAction(actionZoomOut);
        toolBar->addSeparator();
        toolBar->addAction(actionSnapMode);
        toolBar->addAction(actionGridSnapMode);
        toolBar->addSeparator();
        toolBar->addAction(actionLowerEnvelope);
        toolBar->addAction(actionUpperEnvelope);
        toolBar->addSeparator();
        toolBar->addAction(actionConicSegment);
        toolBar->addAction(actionCurveRay);
        toolBar->addAction(actionCurveLine);
        toolBar->addAction(actionConicCircle);
        toolBar->addAction(actionConicEllipse);
        toolBar->addAction(actionConicThreePoint);
        toolBar->addAction(actionConicFivePoint);
        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menu_Edit->menuAction());
        menubar->addAction(menuMode->menuAction());
        menubar->addAction(menuT_ools->menuAction());
        menu_File->addAction(actionNewTab);
        menu_File->addAction(actionCloseTab);
        menu_File->addSeparator();
        menu_File->addAction(actionOpen);
        menu_File->addAction(actionSaveAs);
        menu_File->addSeparator();
        menu_File->addAction(actionQuit);
        menuMode->addAction(actionDrag);
        menuMode->addAction(actionInsert);
        menuMode->addAction(actionDelete);
        menuMode->addAction(actionPointLocation);
        menuMode->addAction(actionRayShootingUp);
        menuMode->addAction(actionRayShootingDown);
        menuMode->addAction(actionMerge);
        menuMode->addAction(actionSplit);
        menuMode->addAction(actionFill);
        menu_Edit->addAction(actionSnapMode);
        menu_Edit->addAction(actionGridSnapMode);
        menu_Edit->addSeparator();
        menu_Edit->addAction(actionShowGrid);
        menu_Edit->addSeparator();
        menu_Edit->addAction(actionPreferences);
        menuT_ools->addAction(actionOverlay);

        retranslateUi(ArrangementDemoWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(ArrangementDemoWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ArrangementDemoWindow)
    {
        ArrangementDemoWindow->setWindowTitle(QApplication::translate("ArrangementDemoWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("ArrangementDemoWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        actionInsert->setText(QApplication::translate("ArrangementDemoWindow", "&Insert", 0, QApplication::UnicodeUTF8));
        actionDelete->setText(QApplication::translate("ArrangementDemoWindow", "&Delete", 0, QApplication::UnicodeUTF8));
        actionDrag->setText(QApplication::translate("ArrangementDemoWindow", "Drag", 0, QApplication::UnicodeUTF8));
        actionPointLocation->setText(QApplication::translate("ArrangementDemoWindow", "Point Location", 0, QApplication::UnicodeUTF8));
        actionRayShootingUp->setText(QApplication::translate("ArrangementDemoWindow", "Ray Shooting Up", 0, QApplication::UnicodeUTF8));
        actionRayShootingDown->setText(QApplication::translate("ArrangementDemoWindow", "Ray Shooting Down", 0, QApplication::UnicodeUTF8));
        actionMerge->setText(QApplication::translate("ArrangementDemoWindow", "Merge", 0, QApplication::UnicodeUTF8));
        actionSplit->setText(QApplication::translate("ArrangementDemoWindow", "Split", 0, QApplication::UnicodeUTF8));
        actionFill->setText(QApplication::translate("ArrangementDemoWindow", "Fill", 0, QApplication::UnicodeUTF8));
        actionSnapMode->setText(QApplication::translate("ArrangementDemoWindow", "Enable snapping", 0, QApplication::UnicodeUTF8));
        actionGridSnapMode->setText(QApplication::translate("ArrangementDemoWindow", "Snap to grid", 0, QApplication::UnicodeUTF8));
        actionSegment->setText(QApplication::translate("ArrangementDemoWindow", "Segment", 0, QApplication::UnicodeUTF8));
        actionPolyline->setText(QApplication::translate("ArrangementDemoWindow", "Polyline", 0, QApplication::UnicodeUTF8));
        actionConic->setText(QApplication::translate("ArrangementDemoWindow", "Conic", 0, QApplication::UnicodeUTF8));
        actionNewTab->setText(QApplication::translate("ArrangementDemoWindow", "New tab", 0, QApplication::UnicodeUTF8));
        actionNewTab->setShortcut(QApplication::translate("ArrangementDemoWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionOverlay->setText(QApplication::translate("ArrangementDemoWindow", "&Overlay...", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("ArrangementDemoWindow", "&Preferences...", 0, QApplication::UnicodeUTF8));
        actionShowGrid->setText(QApplication::translate("ArrangementDemoWindow", "Show &grid", 0, QApplication::UnicodeUTF8));
        actionLowerEnvelope->setText(QApplication::translate("ArrangementDemoWindow", "Lower envelope", 0, QApplication::UnicodeUTF8));
        actionUpperEnvelope->setText(QApplication::translate("ArrangementDemoWindow", "Upper envelope", 0, QApplication::UnicodeUTF8));
        actionConicSegment->setText(QApplication::translate("ArrangementDemoWindow", "Segment", 0, QApplication::UnicodeUTF8));
        actionConicCircle->setText(QApplication::translate("ArrangementDemoWindow", "Circle", 0, QApplication::UnicodeUTF8));
        actionConicEllipse->setText(QApplication::translate("ArrangementDemoWindow", "Ellipse", 0, QApplication::UnicodeUTF8));
        actionConicThreePoint->setText(QApplication::translate("ArrangementDemoWindow", "Three Point Conic", 0, QApplication::UnicodeUTF8));
        actionConicFivePoint->setText(QApplication::translate("ArrangementDemoWindow", "Five Point Conic", 0, QApplication::UnicodeUTF8));
        actionCloseTab->setText(QApplication::translate("ArrangementDemoWindow", "Close Tab", 0, QApplication::UnicodeUTF8));
        actionCloseTab->setShortcut(QApplication::translate("ArrangementDemoWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        actionSaveAs->setText(QApplication::translate("ArrangementDemoWindow", "Save As...", 0, QApplication::UnicodeUTF8));
        actionSaveAs->setShortcut(QApplication::translate("ArrangementDemoWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("ArrangementDemoWindow", "Open...", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("ArrangementDemoWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionPrintConicCurves->setText(QApplication::translate("ArrangementDemoWindow", "Print conic curves", 0, QApplication::UnicodeUTF8));
        actionZoomIn->setText(QApplication::translate("ArrangementDemoWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoomOut->setText(QApplication::translate("ArrangementDemoWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionCurveRay->setText(QApplication::translate("ArrangementDemoWindow", "Ray", 0, QApplication::UnicodeUTF8));
        actionCurveLine->setText(QApplication::translate("ArrangementDemoWindow", "Line", 0, QApplication::UnicodeUTF8));
        actionFillColor->setText(QApplication::translate("ArrangementDemoWindow", "Fill Color", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFillColor->setToolTip(QApplication::translate("ArrangementDemoWindow", "Set the fill color", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolBar->setWindowTitle(QApplication::translate("ArrangementDemoWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("ArrangementDemoWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuMode->setTitle(QApplication::translate("ArrangementDemoWindow", "&Mode", 0, QApplication::UnicodeUTF8));
        menu_Edit->setTitle(QApplication::translate("ArrangementDemoWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        menuT_ools->setTitle(QApplication::translate("ArrangementDemoWindow", "T&ools", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ArrangementDemoWindow: public Ui_ArrangementDemoWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARRANGEMENTDEMOWINDOW_H
