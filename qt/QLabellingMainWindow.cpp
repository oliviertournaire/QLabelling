#include "ui_QLabellingMainWindow.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QSettings>

#include <iostream>

#include "QLabellingMainWindow.hpp"
#include "QLabellingWidget.hpp"
#include "QLabellingView.hpp"

using namespace std;

QLabellingMainWindow::QLabellingMainWindow(QWidget *parent) :
    QMainWindow(parent),
    _labellingWidget(new QLabellingWidget),
    _labelsScene(new QGraphicsScene),
    _labelsView(new QGraphicsView),
    _labelsPixmap(new QPixmap)
{
    _mainWindow = new Ui::QLabellingMainWindow;
    _mainWindow->setupUi(this);

    _labelsView->setScene(_labelsScene);
    _labelsPixmapItem = _labelsScene->addPixmap( _labelsPixmap->fromImage( _labellingWidget->view()->labelsImage() ) );

    QDockWidget* dock = new QDockWidget;
    dock->setWidget(_labellingWidget);
    dock->setWindowTitle( _labellingWidget->windowTitle() );
    dock->setWindowIcon( dock->windowIcon() );
    this->addDockWidget(Qt::RightDockWidgetArea, dock);

    _mainWindow->_tabWidget->insertTab(0, _labellingWidget->view(), tr("Image to label"));
    _mainWindow->_tabWidget->insertTab(1, _labelsView, tr("Labels image"));

    connect(_mainWindow->actionOpen,
            SIGNAL(triggered()),
            this,
            SLOT(openImageToLabel()));

    connect(_mainWindow->actionSaveLabelsImage,
            SIGNAL(triggered()),
            this,
            SLOT(saveLabels()));

    connect(_labellingWidget->view(),
            SIGNAL(labelImageChanged()),
            this,
            SLOT(updateLabelImage()));
}

void QLabellingMainWindow::openImageToLabel()
{
    QSettings settings("IMAGINE", "QSnapEdit");

    settings.beginGroup("QSnapEditMainWindow");
    QString defaultDirectory = settings.value("defaultDirectory", "").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image to label"), defaultDirectory, tr("Image Files (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"));
    _labellingWidget->view()->setImageToLabel(fileName);

    QFileInfo info(fileName);
    settings.setValue("defaultDirectory", info.absolutePath());
    settings.endGroup();
}

void QLabellingMainWindow::saveLabels()
{
    QSettings settings("IMAGINE", "QSnapEdit");

    settings.beginGroup("QSnapEditMainWindow");
    QString defaultDirectory = settings.value("defaultDirectory", "").toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save labels image"), defaultDirectory, tr("Image Files (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"));

    QFileInfo info(fileName);
    settings.setValue("defaultDirectory", info.absolutePath());
    settings.endGroup();

    // Now, save the image
    _labellingWidget->labelsImage().save(fileName);
}

void QLabellingMainWindow::updateLabelImage()
{
    _labelsScene->removeItem(_labelsPixmapItem);
    _labelsPixmapItem = _labelsScene->addPixmap( _labelsPixmap->fromImage( _labellingWidget->view()->labelsImage() ) );
}

QLabellingWidget *QLabellingMainWindow::snapEditWidget() const
{
    return _labellingWidget;
}

void QLabellingMainWindow::setSnapEditWidget(QLabellingWidget *snapEditWidget)
{
    _labellingWidget = snapEditWidget;
}

QGraphicsView *QLabellingMainWindow::labelsView() const
{
    return _labelsView;
}

void QLabellingMainWindow::setLabelsView(QGraphicsView *labelsView)
{
    _labelsView = labelsView;
}

QGraphicsScene *QLabellingMainWindow::labelsScene() const
{
    return _labelsScene;
}

void QLabellingMainWindow::setLabelsScene(QGraphicsScene *labelsScene)
{
    _labelsScene = labelsScene;
}
