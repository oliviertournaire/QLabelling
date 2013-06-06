// Copyright (c) 2012  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: $
// $Id: $
//
// Author(s)     : Alex Tsui <alextsui05@gmail.com>

#include "ArrangementDemoWindow.h"
#include "ArrangementTypes.h"
#include "OverlayDialog.h"
#include "ArrangementDemoPropertiesDialog.h"
#include "ArrangementDemoTab.h"
#include "DeleteCurveMode.h"
#include "ArrangementGraphicsItem.h"

#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QDockWidget>

#include <CGAL/IO/Arr_with_history_iostream.h>
#include <CGAL/IO/Arr_text_formatter.h>
#include <CGAL/IO/Arr_with_history_text_formatter.h>

#ifdef _WINDOWS
// for isinf
#include <boost/math/special_functions/fpclassify.hpp>
#endif // _WINDOWS

#include "QLabellingLogWidget.hpp"
#include "QLabellingWidget.hpp"
#include "config.hpp"

ArrangementDemoWindow::ArrangementDemoWindow(QWidget* parent) :
    CGAL::Qt::DemosMainWindow( parent ),
    lastTabIndex(static_cast<unsigned int>(-1)),
    ui( new Ui::ArrangementDemoWindow ),
    _loggerWidget(QLabellingLogWidget::instance()),
    _labellingWidget(QLabellingWidget::instance())
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    _labellingWidget->setLabelsPath(QLABELLING_DEFAULT_LABEL_PATH);

    this->setupUi( );

    QDockWidget* dockLogWidget = new QDockWidget;
    dockLogWidget->setWidget(_loggerWidget);
    dockLogWidget->setWindowTitle( _loggerWidget->windowTitle() );
    dockLogWidget->setWindowIcon( dockLogWidget->windowIcon() );
    this->addDockWidget(Qt::BottomDockWidgetArea, dockLogWidget);

    QDockWidget* dockLabellingWidget = new QDockWidget;
    dockLabellingWidget->setWidget(_labellingWidget);
    dockLabellingWidget->setWindowTitle( _labellingWidget->windowTitle() );
    dockLabellingWidget->setWindowIcon( dockLogWidget->windowIcon() );
    this->addDockWidget(Qt::RightDockWidgetArea, dockLabellingWidget);

    _loggerWidget->logInfo( tr("QDemoArrangement application started") );

    // set up the demo window
    // ArrangementDemoTabBase* demoTab =
    this->makeTab( POLYLINE_TRAITS );
    this->setupStatusBar( );
    this->setupOptionsMenu( );
    this->addAboutDemo( ":/help/about.html" );
    this->addAboutCGAL( );

    // set up callbacks
    QObject::connect( this->modeGroup, SIGNAL( triggered( QAction* ) ),
                      this, SLOT( updateMode( QAction* ) ) );
    QObject::connect( this->snapGroup, SIGNAL( triggered( QAction* ) ),
                      this, SLOT( updateSnapping( QAction* ) ) );
}

ArrangementDemoTabBase* ArrangementDemoWindow::makeTab( TraitsType tt )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    static int tabLabelCounter = 1;
    QString tabLabel;

    ArrangementDemoTabBase* demoTab;
    Pol_arr* pol_arr;
    CGAL::Object arr;

    switch ( tt )
    {
    default:
    case POLYLINE_TRAITS:
        pol_arr = new Pol_arr;
        demoTab = new ArrangementDemoTab< Pol_arr >( pol_arr, 0 );
        arr = CGAL::make_object( pol_arr );
        tabLabel = QString( "%1 - Polyline" ).arg( tabLabelCounter++ );
        break;
    }

    this->arrangements.push_back( arr );
    this->tabs.push_back( demoTab );

    QGraphicsView* view = demoTab->getView( );
    this->addNavigation( view );
    this->ui->tabWidget->addTab( demoTab, tabLabel );
    this->lastTabIndex = this->ui->tabWidget->currentIndex( );
    this->ui->tabWidget->setCurrentWidget( demoTab );

    this->resetCallbackState( this->ui->tabWidget->currentIndex( ) );
    this->removeCallback( this->ui->tabWidget->currentIndex( ) );
    this->updateMode( this->modeGroup->checkedAction( ) );
    this->updateFillColorSwatch( );

    return demoTab;
}

ArrangementDemoTabBase* ArrangementDemoWindow::getTab( unsigned int tabIndex )
const
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    if (tabIndex > this->tabs.size()) return NULL;
    return this->tabs[tabIndex];
}

ArrangementDemoTabBase* ArrangementDemoWindow::getCurrentTab( ) const
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    int currentIndex = this->ui->tabWidget->currentIndex( );
    if ( currentIndex == -1 )
        return NULL;

    ArrangementDemoTabBase* res = this->tabs[ currentIndex ];
    return res;
}

std::vector< QString > ArrangementDemoWindow::getTabLabels( ) const
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    std::vector< QString > res;
    for ( int i = 0; i < this->ui->tabWidget->count( ); ++i )
    {
        res.push_back( this->ui->tabWidget->tabText( i ) );
    }
    return res;
}

std::vector< CGAL::Object > ArrangementDemoWindow::getArrangements( ) const
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    std::vector< CGAL::Object > res;
    for ( unsigned int i = 0; i < this->arrangements.size( ); ++i )
    {
        res.push_back( this->arrangements[ i ] );
    }
    return res;
}

void ArrangementDemoWindow::setupUi( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    this->ui->setupUi( this );

    this->modeGroup = new QActionGroup( this );
    this->modeGroup->addAction( this->ui->actionDrag );
    this->modeGroup->addAction( this->ui->actionInsert );
    this->modeGroup->addAction( this->ui->actionDelete );
    this->modeGroup->addAction( this->ui->actionPointLocation );
    this->modeGroup->addAction( this->ui->actionMerge );
    this->modeGroup->addAction( this->ui->actionSplit );
    this->modeGroup->addAction( this->ui->actionFill );
    this->activeModes.push_back( this->ui->actionInsert );

    this->snapGroup = new QActionGroup( this );
    this->snapGroup->addAction( this->ui->actionSnapMode );
    this->snapGroup->addAction( this->ui->actionGridSnapMode );
    this->snapGroup->setExclusive( false );
    this->ui->actionGridSnapMode->setEnabled( false );

    this->updateFillColorSwatch( );
}

void ArrangementDemoWindow::updateMode( QAction* newMode )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    // QWidget* widget = this->ui->tabWidget->currentWidget( );
    // ArrangementDemoTabBase* demoTab =
    //   static_cast< ArrangementDemoTabBase* >( widget );
    const unsigned int TabIndex = this->ui->tabWidget->currentIndex( );
    if (TabIndex == static_cast<unsigned int>(-1)) return;
    ArrangementDemoTabBase* activeTab = this->tabs[ TabIndex ];
    QGraphicsScene* activeScene = activeTab->getScene( );
    QGraphicsView* activeView = activeTab->getView( );

    this->resetCallbackState( TabIndex );
    this->removeCallback( TabIndex );

    // update the active mode
    this->activeModes.at( 0 ) = newMode;

    QString messageToLog("Updating mode --> ");

    // hook up the new active mode
    if ( newMode == this->ui->actionInsert )
    {
        activeScene->installEventFilter( activeTab->getCurveInputCallback( ) );
        messageToLog += "Insertion mode";
    }
    else if ( newMode == this->ui->actionDrag )
    {
        activeView->setDragMode( QGraphicsView::ScrollHandDrag );
        messageToLog += "Drag mode";
    }
    else if ( newMode == this->ui->actionDelete )
    {
        activeScene->installEventFilter( activeTab->getDeleteCurveCallback( ) );
        messageToLog += "Delete mode";
    }
    else if ( newMode == this->ui->actionPointLocation )
    {
        activeScene->installEventFilter( activeTab->getPointLocationCallback( ) );
        messageToLog += "Point location mode";
    }
    else if ( newMode == this->ui->actionMerge )
    {
        activeScene->installEventFilter( activeTab->getMergeEdgeCallback( ) );
        messageToLog += "Merge mode";
    }
    else if ( newMode == this->ui->actionSplit )
    {
        activeScene->installEventFilter( activeTab->getSplitEdgeCallback( ) );
        messageToLog += "Split mode";
    }
    else if ( newMode == this->ui->actionFill )
    {
        activeScene->installEventFilter( activeTab->getFillFaceCallback( ) );
        messageToLog += "Fill mode";
    }
    this->updateFillColorSwatch( );

    _loggerWidget->logTrace(messageToLog);
}

void ArrangementDemoWindow::resetCallbackState( unsigned int tabIndex )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    if (tabIndex == static_cast<unsigned int>(-1)
            || tabIndex >= this->tabs.size( )) return;

    ArrangementDemoTabBase* activeTab = this->tabs[ tabIndex ];

    QAction* activeMode = this->activeModes.at( 0 );

    // unhook the old active mode
    if ( activeMode == this->ui->actionInsert )
    {  }
    else if ( activeMode == this->ui->actionDrag )
    {  }
    else if ( activeMode == this->ui->actionDelete )
    {
        activeTab->getDeleteCurveCallback( )->reset( );
    }
    else if ( activeMode == this->ui->actionPointLocation )
    {
        activeTab->getPointLocationCallback( )->reset( );
    }
    else if ( activeMode == this->ui->actionMerge )
    {
        activeTab->getMergeEdgeCallback( )->reset( );
    }
    else if ( activeMode == this->ui->actionSplit )
    {
        activeTab->getSplitEdgeCallback( )->reset( );
    }
    else if ( activeMode == this->ui->actionFill )
    {
        activeTab->getFillFaceCallback( )->reset( );
    }
}

void ArrangementDemoWindow::removeCallback( unsigned int tabIndex )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    if (tabIndex == static_cast<unsigned int>(-1)) return;

    ArrangementDemoTabBase* activeTab = this->tabs[ tabIndex ];
    QGraphicsScene* activeScene = activeTab->getScene( );
    QGraphicsView* activeView = activeTab->getView( );
#if 0
    QAction* activeMode = this->activeModes[ tabIndex ];
#endif

    activeScene->removeEventFilter( activeTab->getCurveInputCallback( ) );
    activeView->setDragMode( QGraphicsView::NoDrag );
    activeScene->removeEventFilter( activeTab->getDeleteCurveCallback( ) );
    activeScene->removeEventFilter( activeTab->getPointLocationCallback( ) );
    activeScene->removeEventFilter( activeTab->getMergeEdgeCallback( ) );
    activeScene->removeEventFilter( activeTab->getSplitEdgeCallback( ) );
    activeScene->removeEventFilter( activeTab->getFillFaceCallback( ) );
}

void ArrangementDemoWindow::updateFillColorSwatch( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    unsigned int currentTabIndex = this->ui->tabWidget->currentIndex( );
    if (currentTabIndex == static_cast<unsigned int>(-1)) return;
    ArrangementDemoTabBase* currentTab = this->tabs[ currentTabIndex ];
    FillFaceCallbackBase* fillFaceCallback = currentTab->getFillFaceCallback( );
    QColor fillColor = fillFaceCallback->getColor( );
    if ( !fillColor.isValid( ) )
    {
        fillColor = ::Qt::black;
    }

    QPixmap fillColorPixmap( 16, 16 );
    fillColorPixmap.fill( fillColor );
    QIcon fillColorIcon( fillColorPixmap );
    this->ui->actionFillColor->setIcon( fillColorIcon );
}

void ArrangementDemoWindow::openArrFile( QString filename )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    int index = this->ui->tabWidget->currentIndex( );
    if ( index == -1 )
    {
        QMessageBox::information( this, "Oops", "Create a new tab first" );
        return;
    }
    if ( filename.isNull( ) )
    {
        return;
    }

    std::ifstream ifs( filename.toStdString( ).c_str( ) );
    CGAL::Object arr = this->arrangements[ index ];
    Pol_arr* pol;
    if ( CGAL::assign( pol, arr ) )
    {
        typedef CGAL::Arr_text_formatter< Pol_arr >         Pol_text_formatter;
        typedef CGAL::Arr_with_history_text_formatter<Pol_text_formatter>
                ArrFormatter;
        typedef ArrangementDemoTab< Pol_arr >               TabType;

        ArrFormatter arrFormatter;
        CGAL::read( *pol, ifs, arrFormatter );
        this->arrangements[ index ] = CGAL::make_object( pol );
        TabType* tab = static_cast< TabType* >( this->tabs[ index ] );
        tab->setArrangement( pol );
    }
    ifs.close( );
}

void ArrangementDemoWindow::openDatFile( QString filename )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    int index = this->ui->tabWidget->currentIndex( );
    if ( index == -1 )
    {
        QMessageBox::information( this, "Oops", "Create a new tab first" );
        return;
    }
    if ( filename.isNull( ) )
    {
        return;
    }

    std::ifstream inputFile( filename.toStdString( ).c_str( ) );
    CGAL::Object arr = this->arrangements[ index ];
    Pol_arr* pol;

    // Creates an ofstream object named inputFile
    if (! inputFile.is_open() ) // Always test file open
    {
        std::cerr << "Error opening input file" << std::endl;
        return;
    }

    if ( CGAL::assign( pol, arr ) )
    {
        pol->clear( );

        std::vector<Arr_pol_point_2> points;

        unsigned int num_polylines;
        inputFile >> num_polylines;
        std::list<Arr_pol_2> pol_list;

        unsigned int i;
        for (i = 0; i < num_polylines; i++)
        {
            unsigned int num_segments;
            inputFile >> num_segments;
            points.clear();
            unsigned int j;
            for (j = 0; j < num_segments; j++)
            {
                int ix, iy;
                inputFile >> ix >> iy;
                points.push_back (Arr_pol_point_2(CGAL::to_double(ix),CGAL::to_double(iy)));
            }

            Arr_pol_2 curve (points.begin(), points.end());
            pol_list.push_back(curve);
        }
        CGAL::insert(*pol, pol_list.begin(), pol_list.end());

        typedef ArrangementDemoTab< Pol_arr > TabType;
        TabType* tab = static_cast< TabType* >( this->tabs[ index ] );
        tab->setArrangement( pol );
    }

    inputFile.close();
}

void ArrangementDemoWindow::updateSnapping( QAction* newMode )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    ArrangementDemoTabBase* activeTab =
            this->tabs[ this->ui->tabWidget->currentIndex( ) ];
    QGraphicsScene* activeScene = activeTab->getScene( );
    ArrangementDemoGraphicsView* activeView = activeTab->getView( );

    bool enabled = newMode->isChecked( );
    if ( newMode == this->ui->actionSnapMode )
    {
        activeTab->getCurveInputCallback( )->setSnappingEnabled( enabled );
        activeTab->getSplitEdgeCallback( )->setSnappingEnabled( enabled );
        if ( ! enabled )
        {
            this->ui->actionGridSnapMode->setChecked( false );
            this->ui->actionGridSnapMode->setEnabled( false );
            activeTab->getCurveInputCallback( )->setSnapToGridEnabled( false );
            activeTab->getSplitEdgeCallback( )->setSnapToGridEnabled( false );
        }
        else
        {
            this->ui->actionGridSnapMode->setEnabled( true );
        }
    }
    else if ( newMode == this->ui->actionGridSnapMode )
    {
        activeTab->getCurveInputCallback( )->setSnapToGridEnabled( enabled );
        activeTab->getSplitEdgeCallback( )->setSnapToGridEnabled( enabled );
        activeView->setShowGrid( enabled );
    }
    activeScene->update( );
}

void ArrangementDemoWindow::on_actionSaveAs_triggered( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    int index = this->ui->tabWidget->currentIndex( );
    if ( index == -1 )
        return;
    QString filename =
            QFileDialog::getSaveFileName( this, tr( "Save file" ),
                                          "", "Arrangement (*.arr)" );
    if ( filename.isNull( ) )
        return;

    std::ofstream ofs( filename.toStdString( ).c_str( ) );
    CGAL::Object arr = this->arrangements[ index ];
    Pol_arr* pol;
    if ( CGAL::assign( pol, arr ) )
    {
        typedef CGAL::Arr_text_formatter<Pol_arr>           Pol_text_formatter;
        typedef CGAL::Arr_with_history_text_formatter<Pol_text_formatter>
                ArrFormatter;
        ArrFormatter                                        arrFormatter;
        CGAL::write( *pol, ofs, arrFormatter );
    }
    ofs.close( );
}

void ArrangementDemoWindow::on_actionOpen_triggered( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    int index = this->ui->tabWidget->currentIndex( );
    if ( index == -1 )
    {
        QMessageBox::information( this, "Oops", "Create a new tab first" );
        return;
    }
    QString filename =
            QFileDialog::getOpenFileName( this, tr( "Open file" ),
                                          "", "Arrangement files (*.arr *.dat);;All files (*.*)" );
    if ( filename.isNull( ) )
        return;

    if ( filename.endsWith( ".arr" ) )
    {
        this->openArrFile( filename );
    }
    else
    {
        this->openDatFile( filename );
    }

    ArrangementDemoTabBase* currentTab = this->tabs[ index ];
    CGAL::Qt::ArrangementGraphicsItemBase* agi =
            currentTab->getArrangementGraphicsItem( );
    QRectF bb = agi->boundingRect( );
    QGraphicsView* view = currentTab->getView( );
    // std::cout << bb.left( ) << " " << bb.bottom( ) << ", " << bb.right( )
    //           << " " << bb.top( ) << std::endl;
#ifndef _WINDOWS
    if ( std::isinf(bb.left( )) ||
         std::isinf(bb.right( )) ||
         std::isinf(bb.top( )) ||
         std::isinf(bb.bottom( )) )
#else
    if ( boost::math::isinf(bb.left( )) ||
         boost::math::isinf(bb.right( )) ||
         boost::math::isinf(bb.top( )) ||
         boost::math::isinf(bb.bottom( )) )
#endif // _WINDOWS
    {
        // std::cout << "unbounded; using default bb" << std::endl;
        bb = QRectF( -100, -100, 200, 200 );
        view->setSceneRect( bb );
    }
    else
    {
        view->fitInView( bb, ::Qt::KeepAspectRatio );
        view->setSceneRect( bb );
    }
#if 0
    view->centerOn( bb.center( ) );
#endif
}

void ArrangementDemoWindow::on_actionQuit_triggered( )
{
    _loggerWidget->logInfo( tr("Quitting QDemoArrangement application") );
    qApp->exit( );
}

void ArrangementDemoWindow::on_actionNewTab_triggered( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    this->makeTab( POLYLINE_TRAITS );
}

void ArrangementDemoWindow::on_tabWidget_currentChanged( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    // std::cout << "Tab changed" << std::endl;
    // disable the callback for the previously active tab
    this->resetCallbackState( this->lastTabIndex );
    this->removeCallback( this->lastTabIndex );
    this->lastTabIndex = this->ui->tabWidget->currentIndex( );

    this->updateMode( this->modeGroup->checkedAction( ) );

    CGAL::Object arr;
    if ( this->ui->tabWidget->currentIndex( ) != -1 )
        arr = this->arrangements[ this->ui->tabWidget->currentIndex( ) ];
}

void ArrangementDemoWindow::on_actionOverlay_triggered( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    OverlayDialog* overlayDialog = new OverlayDialog( this );
    if ( overlayDialog->exec( ) == QDialog::Accepted )
    {
        std::vector< CGAL::Object > arrs = overlayDialog->selectedArrangements( );
        if ( arrs.size( ) == 2 )
        {
            Pol_arr* pol_arr;
            Pol_arr* pol_arr2;
            if ( CGAL::assign( pol_arr, arrs[ 0 ] ) &&
                 CGAL::assign( pol_arr2, arrs[ 1 ] ) )
            {
                this->makeOverlayTab( pol_arr, pol_arr2 );
            }
        }
    }
    delete overlayDialog;
}

void ArrangementDemoWindow::on_actionCloseTab_triggered( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    unsigned int currentTabIndex = this->ui->tabWidget->currentIndex( );
    if (! this->ui->tabWidget->count() ||
            (currentTabIndex == static_cast<unsigned int>(-1)))
        return;

    // delete the tab
    this->ui->tabWidget->removeTab( currentTabIndex );
    this->tabs.erase( this->tabs.begin( ) + currentTabIndex );

    // delete the arrangement
    this->arrangements.erase( this->arrangements.begin( ) + currentTabIndex );
}

void ArrangementDemoWindow::on_actionZoomIn_triggered( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    QLabellingLogWidget::instance()->logDebug( tr("Zoomed in, current scale = ") + QString::number(view->transform().m11()) + " et " + QString::number(view->transform().m22()) );

    unsigned int currentTabIndex = this->ui->tabWidget->currentIndex( );
    if (currentTabIndex == static_cast<unsigned int>(-1)) return;
    ArrangementDemoTabBase* currentTab = this->tabs[ currentTabIndex ];
    QGraphicsView* view = currentTab->getView( );
    view->scale( 2.0, 2.0 );
}

void ArrangementDemoWindow::on_actionZoomOut_triggered( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    QLabellingLogWidget::instance()->logDebug( tr("Zoomed out, current scale = ") + QString::number(view->transform().m11()) + " et " + QString::number(view->transform().m22()) );

    unsigned int currentTabIndex = this->ui->tabWidget->currentIndex( );
    if (currentTabIndex == static_cast<unsigned int>(-1)) return;
    ArrangementDemoTabBase* currentTab = this->tabs[ currentTabIndex ];
    QGraphicsView* view = currentTab->getView( );
    view->scale( 0.5, 0.5 );
}

void ArrangementDemoWindow::on_actionPreferences_triggered( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    unsigned int currentTabIndex = this->ui->tabWidget->currentIndex( );
    if (currentTabIndex == static_cast<unsigned int>(-1)) return;
    ArrangementDemoTabBase* currentTab = this->tabs[ currentTabIndex ];
    CGAL::Qt::ArrangementGraphicsItemBase* agi =
            currentTab->getArrangementGraphicsItem( );
    ArrangementDemoGraphicsView* view = currentTab->getView( );
    SplitEdgeCallbackBase* splitEdgeCallback = currentTab->getSplitEdgeCallback( );

    ArrangementDemoPropertiesDialog* dialog =
            new ArrangementDemoPropertiesDialog( this );
    if ( dialog->exec( ) == QDialog::Accepted )
    {
        typedef ArrangementDemoPropertiesDialog Dialog;
        QColor edgeColor =
                qVariantValue<QColor>(dialog->property(Dialog::EDGE_COLOR_KEY));
        unsigned int edgeWidth =
                qVariantValue<unsigned int>(dialog->property(Dialog::EDGE_WIDTH_KEY));
        QColor vertexColor =
                qVariantValue<QColor>(dialog->property(Dialog::VERTEX_COLOR_KEY));
        unsigned int vertexRadius =
                qVariantValue<unsigned int>(dialog->property(Dialog::VERTEX_RADIUS_KEY));
        DeleteCurveMode mode =
                qVariantValue<DeleteCurveMode>(dialog->property(Dialog::
                                                                DELETE_CURVE_MODE_KEY));
        unsigned int gridSize =
                qVariantValue<unsigned int>(dialog->property(Dialog::GRID_SIZE_KEY));
        QColor gridColor =
                qVariantValue<QColor>(dialog->property(Dialog::GRID_COLOR_KEY));

        QPen edgesPen(QBrush(edgeColor), edgeWidth);
        QPen verticesPen(QBrush(vertexColor), vertexRadius);
        agi->setEdgesPen( edgesPen );
        agi->setVerticesPen( verticesPen );
        agi->modelChanged( );
        view->setGridSize( gridSize );
        view->setGridColor( gridColor );
        splitEdgeCallback->setColor( edgeColor );
    }
}

void ArrangementDemoWindow::on_actionFillColor_triggered( )
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    unsigned int currentTabIndex = this->ui->tabWidget->currentIndex( );
    if (currentTabIndex == static_cast<unsigned int>(-1)) return;
    ArrangementDemoTabBase* currentTab = this->tabs[ currentTabIndex ];
    FillFaceCallbackBase* fillFaceCallback = currentTab->getFillFaceCallback( );
    QColor fillColor = fillFaceCallback->getColor( );

    QColor selectedColor = QColorDialog::getColor( fillColor );
    if ( selectedColor.isValid( ) )
    {
        fillFaceCallback->setColor( selectedColor );
        this->updateFillColorSwatch( );
    }
}

void ArrangementDemoWindow::on_actionOpenImage_triggered()
{
    QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    // TODO : vérifier si on est sur un onglet valide !
    
    QSettings settings(QLABELLING_ORGANIZATION_STRING, QLABELLING_NAME_STRING);

    settings.beginGroup("QLabellingMainWindow");
    QString defaultDirectory = settings.value("defaultDirectory", "").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image to label"), defaultDirectory, tr("Image Files (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"));
    if(!fileName.isNull())
    {
        //         _labellingWidget->view()->setImageToLabel(fileName);
        QFileInfo info(fileName);
        settings.setValue("defaultDirectory", info.absolutePath());
        QLabellingLogWidget::instance()->logTrace( QString("Open image " + fileName) );

        QGraphicsScene* tabScene = getCurrentTab()->getScene();
        ArrangementDemoGraphicsView* tabView = getCurrentTab()->getView();
        tabView->_imageToLabel = QPixmap(fileName);
        if(tabView->_imageToLabel.isNull())
        {
            _loggerWidget->logError( tr("Unable to open image ") + fileName );
            return;
        }
        tabView->_imageToLabelFilename = fileName;
        tabScene->addPixmap(tabView->_imageToLabel); /*->setPos(-tabView->_imageToLabel.width()/2 , -tabView->_imageToLabel.height()/2)*/
            
            // On centre la vue
//             tabView->translate(tabView->_imageToLabel.width()/2 , tabView->_imageToLabel.height()/2);

// 	        Arr_pol_point_2 ptl(-tabView->_imageToLabel.width()/2, -tabView->_imageToLabel.height()/2),
//                 pbl(-tabView->_imageToLabel.width()/2, tabView->_imageToLabel.height()/2),
//                 pbr(tabView->_imageToLabel.width()/2, tabView->_imageToLabel.height()/2),
//                 ptr(tabView->_imageToLabel.width()/2, -tabView->_imageToLabel.height()/2);
        Arr_pol_point_2 ptl( 0, 0),
                pbl(0, tabView->_imageToLabel.height() ),
                pbr(tabView->_imageToLabel.width(), tabView->_imageToLabel.height()),
                ptr( tabView->_imageToLabel.width(), 0 );

        QString imageBoundaryMessage = tr("Image boundaries: ");
        imageBoundaryMessage = imageBoundaryMessage + "(" + QString::number(CGAL::to_double(ptl.x())) + "," + QString::number(CGAL::to_double(ptl.y())) + ") / ";
        imageBoundaryMessage = imageBoundaryMessage + "(" + QString::number(CGAL::to_double(pbl.x())) + "," + QString::number(CGAL::to_double(pbl.y())) + ") / ";
        imageBoundaryMessage = imageBoundaryMessage + "(" + QString::number(CGAL::to_double(pbr.x())) + "," + QString::number(CGAL::to_double(pbr.y())) + ") / ";
        imageBoundaryMessage = imageBoundaryMessage + "(" + QString::number(CGAL::to_double(ptr.x())) + "," + QString::number(CGAL::to_double(ptr.y())) + ")";
        _loggerWidget->logTrace( imageBoundaryMessage );

        std::vector<Arr_pol_point_2> allPoints;
        allPoints.push_back(ptl);
        allPoints.push_back(pbl);
        allPoints.push_back(pbr);
        allPoints.push_back(ptr);
        allPoints.push_back(ptl);

        Arr_pol_2 contour(allPoints.begin(), allPoints.end());
	
// 	tabScene->sceneRect()
// 	setFixedSize(tabView->_imageToLabel.width()*1.1 , tabView->_imageToLabel.height()*1.1);

        Pol_arr *arr;
        const unsigned int TabIndex = this->ui->tabWidget->currentIndex( );
        if (TabIndex == static_cast<unsigned int>(-1))
            return;
        ArrangementDemoTabBase* activeTab = this->tabs[ TabIndex ];

        if ( CGAL::assign( arr, getArrangements()[TabIndex] ) )
        {
            CGAL::Qt::GraphicsViewCurveInputBase *gvcib = activeTab->getCurveInputCallback();
            ArrangementCurveInputCallback<Pol_arr> *acic = dynamic_cast< ArrangementCurveInputCallback<Pol_arr>* >(gvcib);
            if(acic)
            {
                acic->processInput( CGAL::make_object(contour) );
            }
        }
        else
        {
            _loggerWidget->logWarning( tr("Unable to retrieve the arrangement!") );
        }
        
        qreal x1, y1, w, h;
        tabScene->sceneRect().getRect(&x1, &y1, &w, &h);
        _loggerWidget->logDebug( "SceneRect = " + QString::number(x1) + ":" + QString::number(y1) + " - " + QString::number(w) + ":" + QString::number(h) );
	tabView->ensureVisible(0,0,tabView->_imageToLabel.width(), tabView->_imageToLabel.height());
// 	tabView->centerOn(tabView->_imageToLabel.width()/2, tabView->_imageToLabel.height()/2); Useless
        tabView->sceneRect().setRect(-10 , -10, tabView->frameSize().width()+20,tabView->frameSize().height()+20);
        tabScene->sceneRect().getRect(&x1, &y1, &w, &h);
        _loggerWidget->logDebug( "SceneRect = " + QString::number(x1) + ":" + QString::number(y1) + " - " + QString::number(w) + ":" + QString::number(h) );
    }
    settings.endGroup();
}

