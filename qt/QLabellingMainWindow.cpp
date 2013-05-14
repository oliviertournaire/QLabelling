#include "ui_QLabellingMainWindow.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>

#include <iostream>
#include <fstream>

#include "QZoomableGraphicsView.hpp"
#include "QLabellingMainWindow.hpp"
#include "QLabellingWidget.hpp"
#include "QLabellingView.hpp"
#include "QLabellingAbout.hpp"
#include "config.hpp"

using namespace std;

QLabellingMainWindow::QLabellingMainWindow(QWidget *parent) :
    QMainWindow(parent),
    _labellingWidget(new QLabellingWidget),
    _labelsScene(new QGraphicsScene),
    _labelsView(new QZoomableGraphicsView),
    _labelsPixmap(new QPixmap),
    _labelsPixmapSaved(true)
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

    connectAll();
}

QLabellingMainWindow::~QLabellingMainWindow()
{
    disconnectAll();
}

void QLabellingMainWindow::connectAll()
{
    connect(_mainWindow->actionOpen,
            SIGNAL(triggered()),
            this,
            SLOT(openImageToLabel()));

    connect(_mainWindow->actionSaveLabelsImage,
            SIGNAL(triggered()),
            this,
            SLOT(saveLabels()));

    connect(_mainWindow->actionAbout,
            SIGNAL(triggered()),
            this,
            SLOT(showAbout()));

    connect(_labellingWidget->view(),
            SIGNAL(labelImageChanged()),
            this,
            SLOT(updateLabelImage()));

    connect(_mainWindow->actionQuit,
            SIGNAL(triggered()),
            this,
            SLOT(quit()));

    connect(qApp,
            SIGNAL(aboutToQuit()),
            this,
            SLOT(quit()));
}

void QLabellingMainWindow::disconnectAll()
{
    disconnect(_mainWindow->actionOpen,
               SIGNAL(triggered()),
               this,
               SLOT(openImageToLabel()));

    disconnect(_mainWindow->actionSaveLabelsImage,
               SIGNAL(triggered()),
               this,
               SLOT(saveLabels()));

    disconnect(_mainWindow->actionAbout,
               SIGNAL(triggered()),
               this,
               SLOT(showAbout()));

    disconnect(_labellingWidget->view(),
               SIGNAL(labelImageChanged()),
               this,
               SLOT(updateLabelImage()));

    disconnect(_mainWindow->actionQuit,
               SIGNAL(triggered()),
               this,
               SLOT(quit()));
}

void QLabellingMainWindow::closeEvent(QCloseEvent *event)
{
    if (_labelsPixmapSaved)
        event->accept();
    else
        quit();
}

void QLabellingMainWindow::openImageToLabel()
{
    QSettings settings(QLABELLING_ORGANIZATION_STRING, QLABELLING_NAME_STRING);

    settings.beginGroup("QLabellingMainWindow");
    QString defaultDirectory = settings.value("defaultDirectory", "").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image to label"), defaultDirectory, tr("Image Files (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"));
    _labellingWidget->view()->setImageToLabel(fileName);

    QFileInfo info(fileName);
    settings.setValue("defaultDirectory", info.absolutePath());
    settings.endGroup();
}

void QLabellingMainWindow::saveLabels()
{
    QSettings settings(QLABELLING_ORGANIZATION_STRING, QLABELLING_NAME_STRING);

    settings.beginGroup("QLabellingMainWindow");
    QString defaultDirectory = settings.value("defaultDirectory", "").toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save labels image"), defaultDirectory, tr("Image Files (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"));

    QFileInfo info(fileName);
    // Here, we automatically add a ".tif" extension if the user did not select explicitely an extension
    if ( info.completeSuffix() == "" )
        fileName += ".tif";
    settings.setValue("defaultDirectory", info.absolutePath());
    settings.endGroup();

    // Now, save the image
    _labellingWidget->labelsImage().save(fileName);
    // And a file which stores labelling infos
    QString labelsFilename = info.filePath();
    labelsFilename += ".labels";
    ofstream of(labelsFilename.toStdString().c_str());
    if(of.good())
    {
        QLabellingView* v = _labellingWidget->view();
        of << v->imageToLabelFilename().toStdString() << endl;
        of << fileName.toStdString() << endl;
        // Task #2: begins here!!!
        of << "Window " << v->windowColor().red() << " " << v->windowColor().green() << " " << v->windowColor().blue() << " " << v->windowColor().alpha() << endl;
        of << "Wall " << v->wallColor().red() << " " << v->wallColor().green() << " " << v->wallColor().blue() << " " << v->wallColor().alpha() << endl;
        of << "Balcony " << v->balconyColor().red() << " " << v->balconyColor().green() << " " << v->balconyColor().blue() << " " << v->balconyColor().alpha() << endl;
        of << "Door " << v->doorColor().red() << " " << v->doorColor().green() << " " << v->doorColor().blue() << " " << v->doorColor().alpha() << endl;
        of << "Shop " << v->shopColor().red() << " " << v->shopColor().green() << " " << v->shopColor().blue() << " " << v->shopColor().alpha() << endl;
        of << "Roof " << v->roofColor().red() << " " << v->roofColor().green() << " " << v->roofColor().blue() << " " << v->roofColor().alpha() << endl;
        of << "Sky " << v->skyColor().red() << " " << v->skyColor().green() << " " << v->skyColor().blue() << " " << v->skyColor().alpha() << endl;
        of << "Unknow " << v->unknowColor().red() << " " << v->unknowColor().green() << " " << v->unknowColor().blue() << " " << v->unknowColor().alpha() << endl;
        // Task #2: end here!!!
        of.close();
    }
    _labelsPixmapSaved = true;
}

void QLabellingMainWindow::showAbout()
{
    QLabellingAbout about;
    about.exec();
}

void QLabellingMainWindow::updateLabelImage()
{
    _labelsScene->removeItem(_labelsPixmapItem);
    _labelsPixmapItem = _labelsScene->addPixmap( _labelsPixmap->fromImage( _labellingWidget->view()->labelsImage() ) );
    _labelsPixmapSaved = false;
}

void QLabellingMainWindow::quit()
{
    if (!_labelsPixmapSaved)
    {
        QMessageBox msgBox;
        msgBox.setText( tr("Labels image has been modified but not saved!") );
        msgBox.setInformativeText( tr("Do you want to save your changes?") );
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();

        switch (ret)
        {
        case QMessageBox::Save:
            // Save was clicked: save labels image
            saveLabels();
            break;
        case QMessageBox::Discard:
            // Don't Save was clicked: exit
            qApp->quit();
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked: do nothing
            break;
        default:
            // should never be reached
            break;
        }
    }
    qApp->quit();
}

QLabellingWidget *QLabellingMainWindow::snapEditWidget() const
{
    return _labellingWidget;
}

void QLabellingMainWindow::setSnapEditWidget(QLabellingWidget *snapEditWidget)
{
    _labellingWidget = snapEditWidget;
}

QZoomableGraphicsView *QLabellingMainWindow::labelsView() const
{
    return _labelsView;
}

void QLabellingMainWindow::setLabelsView(QZoomableGraphicsView *labelsView)
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

Ui::QLabellingMainWindow *QLabellingMainWindow::mainWindow() const
{
    return _mainWindow;
}

void QLabellingMainWindow::setMainWindow(Ui::QLabellingMainWindow *mainWindow)
{
    _mainWindow = mainWindow;
}
