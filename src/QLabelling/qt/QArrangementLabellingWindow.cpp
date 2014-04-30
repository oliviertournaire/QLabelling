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

#include "QArrangementLabellingWindow.h"

#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QDockWidget>
#include <QXmlStreamWriter>

#include <CGAL/IO/Arr_with_history_iostream.h>
#include <CGAL/IO/Arr_text_formatter.h>
#include <CGAL/IO/Arr_with_history_text_formatter.h>

#ifdef _WINDOWS
// for isinf
#include <boost/math/special_functions/fpclassify.hpp>
#endif // _WINDOWS

#include "ArrangementTypes.h"
#include "QArrangementLabellingOverlayDialog.h"
#include "QArrangementLabellingPropertiesDialog.h"
#include "QArrangementLabellingSaveProjectDialog.h"
#include "QArrangementLabellingGraphicsView.h"
#include "QArrangementLabellingTab.h"
#include "ArrangementGraphicsItem.h"
#include "QArrangementLabellingLogWidget.h"
#include "QArrangementLabellingWidget.h"
#include "QArrangementLabellingInfoWidget.h"
#include "ArrangementCurveInputCallback.h"
#include "FillFaceCallback.h"
#include "DeleteCurveMode.h"
#include "config.hpp"

QArrangementLabellingWindow::QArrangementLabellingWindow(QWidget* parent) :
CGAL::Qt::DemosMainWindow( parent ),
    _lastTabIndex(static_cast<unsigned int>(-1)),
    _ui( new Ui::QArrangementLabellingWindow ),
    _loggerWidget(QArrangementLabellingLogWidget::instance()),
    _labellingWidget(QArrangementLabellingWidget::instance())
{
    setAttribute( Qt::WA_AlwaysShowToolTips);
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
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

    QDockWidget* dockInfoWidget = new QDockWidget;
    dockInfoWidget->setWidget(QArrangementLabellingInfoWidget::instance());
    dockInfoWidget->setWindowTitle( QArrangementLabellingInfoWidget::instance()->windowTitle() );
    dockInfoWidget->setWindowIcon( dockInfoWidget->windowIcon() );
    this->addDockWidget(Qt::RightDockWidgetArea, dockInfoWidget);

    _loggerWidget->logInfo( tr("QDemoArrangement application started") );

    // set up the window
    this->makeTab( POLYLINE_TRAITS );
    this->setupStatusBar( );
    this->setupOptionsMenu( );
    this->addAboutDemo( ":/help/about.html" );
    this->addAboutCGAL( );

    // set up callbacks
    QObject::connect( _labellingWidget, SIGNAL( labelChanged() ),
        this, SLOT( labelChanged() ) );
    QObject::connect( this->_modeGroup, SIGNAL( triggered( QAction* ) ),
        this, SLOT( updateMode( QAction* ) ) );
    QObject::connect( this->_snapGroup, SIGNAL( triggered( QAction* ) ),
        this, SLOT( updateSnapping( QAction* ) ) );

    // disable arrangement edition
    updateToolBarButtonsEnable(false);
}

void QArrangementLabellingWindow::labelChanged(){
    _loggerWidget->logTrace( tr("Label just changed, switching current mode to \"fill\"...") );
    this->_ui->actionFill->setChecked(true);
    updateMode(this->_ui->actionFill);
}

QArrangementLabellingTabBase* QArrangementLabellingWindow::makeTab( TraitsType tt )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    static int tabLabelCounter = 1;
    QString tabLabel;

    QArrangementLabellingTabBase* demoTab;
    Pol_arr* pol_arr;
    CGAL::Object arr;

    pol_arr = new Pol_arr;
    demoTab = new QArrangementLabellingTab< Pol_arr >( pol_arr, 0 );
    arr = CGAL::make_object( pol_arr );
    tabLabel = tr( "Labelling (#%n)","", tabLabelCounter++ );

    this->_arrangements.push_back( arr );
    this->_tabs.push_back( demoTab );

    QGraphicsView* view = demoTab->getView( );
    this->addNavigation( view );
    this->_ui->tabWidget->addTab( demoTab, tabLabel );
    this->_lastTabIndex = this->_ui->tabWidget->currentIndex( );
    this->_ui->tabWidget->setCurrentWidget( demoTab );

    this->resetCallbackState( this->_ui->tabWidget->currentIndex( ) );
    this->removeCallback( this->_ui->tabWidget->currentIndex( ) );
    this->updateMode( this->_modeGroup->checkedAction( ) );

    if(!demoTab->_imageHasBeenLoaded){
        QArrangementLabellingLogWidget::instance()->logWarning( tr("Before being able to edit the label arrangement, you must open an image!!!") );
        updateMode( this->_ui->actionDrag );
    }

    return demoTab;
}

QArrangementLabellingTabBase* QArrangementLabellingWindow::getTab( unsigned int tabIndex )
    const
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    if (tabIndex > this->_tabs.size()) return NULL;
    return this->_tabs[tabIndex];
}

QArrangementLabellingTabBase* QArrangementLabellingWindow::getCurrentTab( ) const
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    int currentIndex = this->_ui->tabWidget->currentIndex( );
    if ( currentIndex == -1 )
        return NULL;

    QArrangementLabellingTabBase* res = this->_tabs[ currentIndex ];
    return res;
}

std::vector< QString > QArrangementLabellingWindow::getTabLabels( ) const
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    std::vector< QString > res;
    for ( int i = 0; i < this->_ui->tabWidget->count( ); ++i )
    {
        res.push_back( this->_ui->tabWidget->tabText( i ) );
    }
    return res;
}

std::vector< CGAL::Object > QArrangementLabellingWindow::getArrangements( ) const
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    std::vector< CGAL::Object > res;
    for ( unsigned int i = 0; i < this->_arrangements.size( ); ++i )
    {
        res.push_back( this->_arrangements[ i ] );
    }
    return res;
}

void QArrangementLabellingWindow::setupUi( )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    this->_ui->setupUi( this );

    this->_modeGroup = new QActionGroup( this );
    this->_modeGroup->addAction( this->_ui->actionDrag );
    this->_modeGroup->addAction( this->_ui->actionInsert );
    this->_modeGroup->addAction( this->_ui->actionInsert_horizontal_line );
    this->_modeGroup->addAction( this->_ui->actionInsert_vertical_line );
    this->_modeGroup->addAction( this->_ui->actionDelete );
    this->_modeGroup->addAction( this->_ui->actionPointLocation );
    this->_modeGroup->addAction( this->_ui->actionMerge );
    this->_modeGroup->addAction( this->_ui->actionSplit );
    this->_modeGroup->addAction( this->_ui->actionFill );
    this->_activeModes.push_back(this->_ui->actionDrag );

    this->_snapGroup = new QActionGroup( this );
    this->_snapGroup->addAction( this->_ui->actionSnapMode );
    this->_snapGroup->addAction( this->_ui->actionGridSnapMode );
    this->_snapGroup->setExclusive( false );
    this->_ui->actionGridSnapMode->setEnabled( false );

//     this->updateFillColorSwatch( );
}

void QArrangementLabellingWindow::updateMode( QAction* newMode )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    const unsigned int TabIndex = this->_ui->tabWidget->currentIndex( );
    if (TabIndex == static_cast<unsigned int>(-1)) return;
    QArrangementLabellingTabBase* activeTab = this->_tabs[ TabIndex ];

    QGraphicsScene* activeScene = activeTab->getScene( );
    QGraphicsView* activeView = activeTab->getView( );

    this->resetCallbackState( TabIndex );
    this->removeCallback( TabIndex );

    // update the active mode
    this->_activeModes.at( 0 ) = newMode;

    QString messageToLog(tr("Updating mode --> "));

    // hook up the new active mode
    if ( newMode == this->_ui->actionInsert )
    {
	activeTab->getCurveInputCallback( )->_mode = POLYLINE;
        activeScene->installEventFilter( activeTab->getCurveInputCallback( ) );
        messageToLog += tr("Insertion mode");
    }
    else if ( newMode == this->_ui->actionInsert_horizontal_line  )
    {
	activeTab->getCurveInputCallback( )->_mode = HORIZONTAL;
        activeScene->installEventFilter( activeTab->getCurveInputCallback( ) );
        messageToLog += tr("Insertion (horizontal) mode");
    }
    else if ( newMode == this->_ui->actionInsert_vertical_line )
    {
	activeTab->getCurveInputCallback( )->_mode = VERTICAL;
        activeScene->installEventFilter( activeTab->getCurveInputCallback( ) );
        messageToLog += tr("Insertion (vertical) mode");
    }
    else if ( newMode == this->_ui->actionDrag )
    {
        activeView->setDragMode( QGraphicsView::ScrollHandDrag );
        messageToLog += tr("Drag mode");
    }
    else if ( newMode == this->_ui->actionDelete )
    {
        activeScene->installEventFilter( activeTab->getDeleteCurveCallback( ) );
        messageToLog += tr("Delete mode");
    }
    else if ( newMode == this->_ui->actionPointLocation )
    {
        activeScene->installEventFilter( activeTab->getPointLocationCallback( ) );
        messageToLog += tr("Point location mode");
    }
    else if ( newMode == this->_ui->actionMerge )
    {
        activeScene->installEventFilter( activeTab->getMergeEdgeCallback( ) );
        messageToLog += tr("Merge mode");
    }
    else if ( newMode == this->_ui->actionSplit )
    {
        activeScene->installEventFilter( activeTab->getSplitEdgeCallback( ) );
        messageToLog += tr("Split mode");
    }
    else if ( newMode == this->_ui->actionFill )
    {
        activeScene->installEventFilter( activeTab->getFillFaceCallback( ) );
        messageToLog += tr("Fill mode");
    }
//     this->updateFillColorSwatch( );

    _loggerWidget->logTrace(messageToLog);
}

void QArrangementLabellingWindow::resetCallbackState( unsigned int tabIndex )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    if (tabIndex == static_cast<unsigned int>(-1) || tabIndex >= this->_tabs.size( ))
        return;

    QArrangementLabellingTabBase* activeTab = this->_tabs[ tabIndex ];

    QAction* activeMode = this->_activeModes.at( 0 );

    // unhook the old active mode
    if ( activeMode == this->_ui->actionInsert )
    {  }
    else if ( activeMode == this->_ui->actionInsert_horizontal_line )
    {  }
    else if ( activeMode == this->_ui->actionInsert_vertical_line )
    {  }
    else if ( activeMode == this->_ui->actionDrag )
    {  }
    else if ( activeMode == this->_ui->actionDelete )
    {
        activeTab->getDeleteCurveCallback( )->reset( );
    }
    else if ( activeMode == this->_ui->actionPointLocation )
    {
        activeTab->getPointLocationCallback( )->reset( );
    }
    else if ( activeMode == this->_ui->actionMerge )
    {
        activeTab->getMergeEdgeCallback( )->reset( );
    }
    else if ( activeMode == this->_ui->actionSplit )
    {
        activeTab->getSplitEdgeCallback( )->reset( );
    }
    else if ( activeMode == this->_ui->actionFill )
    {
        activeTab->getFillFaceCallback( )->reset( );
    }
}

void QArrangementLabellingWindow::removeCallback( unsigned int tabIndex )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    if (tabIndex == static_cast<unsigned int>(-1))
        return;

    QArrangementLabellingTabBase* activeTab = this->_tabs[ tabIndex ];
    QGraphicsScene* activeScene = activeTab->getScene( );
    QGraphicsView* activeView = activeTab->getView( );

    activeScene->removeEventFilter( activeTab->getCurveInputCallback( ) );
    activeView->setDragMode( QGraphicsView::NoDrag );
    activeScene->removeEventFilter( activeTab->getDeleteCurveCallback( ) );
    activeScene->removeEventFilter( activeTab->getPointLocationCallback( ) );
    activeScene->removeEventFilter( activeTab->getMergeEdgeCallback( ) );
    activeScene->removeEventFilter( activeTab->getSplitEdgeCallback( ) );
    activeScene->removeEventFilter( activeTab->getFillFaceCallback( ) );
}

void QArrangementLabellingWindow::openArrFile( QString filename )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    int index = this->_ui->tabWidget->currentIndex( );
    if ( index == -1 )
    {
        QMessageBox::information( this, tr("Oops"), tr("Create a new tab first") );
        return;
    }
    if ( filename.isNull( ) )
    {
        return;
    }

    std::ifstream ifs( filename.toStdString( ).c_str( ) );
    CGAL::Object arr = this->_arrangements[ index ];
    Pol_arr* pol;
    if ( CGAL::assign( pol, arr ) )
    {
        typedef CGAL::Arr_face_extended_text_formatter<Pol_arr>           Pol_text_formatter;
        typedef CGAL::Arr_with_history_text_formatter<Pol_text_formatter> ArrFormatter;
        typedef QArrangementLabellingTab< Pol_arr >                       TabType;

        ArrFormatter arrFormatter;
        CGAL::read( *pol, ifs, arrFormatter );
        this->_arrangements[ index ] = CGAL::make_object( pol );
        TabType* tab = static_cast< TabType* >( this->_tabs[ index ] );
        tab->setArrangement( pol );
    }
    ifs.close( );
}

void QArrangementLabellingWindow::openDatFile( QString filename )
{
    QArrangementLabellingLogWidget::instance()->logWarning( QString(__FUNCTION__) + " --> Not implemented " );
}

void QArrangementLabellingWindow::updateSnapping( QAction* newMode )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    QArrangementLabellingTabBase* activeTab =
        this->_tabs[ this->_ui->tabWidget->currentIndex( ) ];

    QGraphicsScene* activeScene = activeTab->getScene( );
    QArrangementLabellingGraphicsView* activeView = activeTab->getView( );

    bool enabled = newMode->isChecked( );
    if ( newMode == this->_ui->actionSnapMode )
    {
        activeTab->getCurveInputCallback( )->setSnappingEnabled( enabled );
        activeTab->getSplitEdgeCallback( )->setSnappingEnabled( enabled );
        if ( ! enabled )
        {
            this->_ui->actionGridSnapMode->setChecked( false );
            this->_ui->actionGridSnapMode->setEnabled( false );
            activeTab->getCurveInputCallback( )->setSnapToGridEnabled( false );
            activeTab->getSplitEdgeCallback( )->setSnapToGridEnabled( false );
        }
        else
        {
            this->_ui->actionGridSnapMode->setEnabled( true );
        }
    }
    else if ( newMode == this->_ui->actionGridSnapMode )
    {
        activeTab->getCurveInputCallback( )->setSnapToGridEnabled( enabled );
        activeTab->getSplitEdgeCallback( )->setSnapToGridEnabled( enabled );
        activeView->setShowGrid( enabled );
    }
    activeScene->update( );
}

// Saving arrangement
void QArrangementLabellingWindow::on_actionSaveAs_triggered( )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    int index = this->_ui->tabWidget->currentIndex( );
    if ( index == -1 )
        return;
    QString filename = QFileDialog::getSaveFileName( this, tr( "Save file" ), "", tr("Arrangement (*.arr)") );
    if ( filename.isNull( ) )
        return;

    doSaveArrangement(filename);
}

void QArrangementLabellingWindow::doSaveArrangement( const QString& filename )
{
    std::ofstream ofs( filename.toStdString( ).c_str( ) );
    CGAL::Object arr = this->_arrangements[ this->_ui->tabWidget->currentIndex( ) ];
    Pol_arr* pol;
    if ( CGAL::assign( pol, arr ) )
    {
        typedef CGAL::Arr_face_extended_text_formatter<Pol_arr>           Pol_text_formatter;
        typedef CGAL::Arr_with_history_text_formatter<Pol_text_formatter> ArrFormatter;
        ArrFormatter arrFormatter;
        CGAL::write( *pol, ofs, arrFormatter );
    }
    this->getCurrentTab()->setArrHasBeenSaved( true );
    ofs.close( );
}

void QArrangementLabellingWindow::on_actionOpen_triggered( )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    int index = this->_ui->tabWidget->currentIndex( );
    if ( index == -1 )
    {
        QMessageBox::information( this, tr("Oops"), tr("Create a new tab first") );
        return;
    }
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open file" ), "", tr("Arrangement files (*.arr *.dat);;All files (*.*)") );
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
}

void QArrangementLabellingWindow::on_actionQuit_triggered( )
{
    _loggerWidget->logInfo( tr("Quitting QDemoArrangement application") );
    qApp->exit( );
}

void QArrangementLabellingWindow::on_actionNewTab_triggered( )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    this->makeTab( POLYLINE_TRAITS );
    
    _ui->actionOpenImage->setEnabled(true);
}

void QArrangementLabellingWindow::on_tabWidget_currentChanged( )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    if(getCurrentTab())
	updateToolBarButtonsEnable(getCurrentTab()->_imageHasBeenLoaded);

    // std::cout << "Tab changed" << std::endl;
    // disable the callback for the previously active tab
    this->resetCallbackState( this->_lastTabIndex );
    this->removeCallback( this->_lastTabIndex );
    this->_lastTabIndex = this->_ui->tabWidget->currentIndex( );

    this->updateMode( this->_modeGroup->checkedAction( ) );

    // Filling Arrangement info widget
    QArrangementLabellingInfoWidget* infoWidget = QArrangementLabellingInfoWidget::instance();
    infoWidget->setNumFaces             ( 0 , 0 );
    infoWidget->setNumEdges             ( 0 );
    infoWidget->setNumVertices          ( 0 );
    infoWidget->setNumIsolatedVertices  ( 0 );
    infoWidget->setNumVerticesAtInfinity( 0 );
    infoWidget->setChanged              ( false );
}

void QArrangementLabellingWindow::on_actionOverlay_triggered( )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    QArrangementLabellingOverlayDialog* overlayDialog = new QArrangementLabellingOverlayDialog( this );
    if ( overlayDialog->exec( ) == QDialog::Accepted )
    {
        std::vector< CGAL::Object > arrs = overlayDialog->selectedArrangements( );
        if ( arrs.size( ) == 2 )
        {
            Pol_arr* pol_arr;
            Pol_arr* pol_arr2;
            if ( CGAL::assign( pol_arr, arrs[ 0 ] ) && CGAL::assign( pol_arr2, arrs[ 1 ] ) )
            {
                this->makeOverlayTab( pol_arr, pol_arr2 );
            }
        }
    }
    delete overlayDialog;
}

void QArrangementLabellingWindow::on_actionCloseTab_triggered( )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    unsigned int currentTabIndex = this->_ui->tabWidget->currentIndex( );
    if (! this->_ui->tabWidget->count() || (currentTabIndex == static_cast<unsigned int>(-1)))
        return;
    
    if(! getCurrentTab()->arrHasBeenSaved() && ! getCurrentTab()->_labelsHaveBeenSaved){ // If labels have already been saved, do not complain
        if(QMessageBox::question(this, tr("Arrangement not saved"), tr("The current arrangement has not been saved, do you really want to close this tab ?"),QMessageBox::Yes, QMessageBox::Cancel) != QMessageBox::Yes){
            QArrangementLabellingLogWidget::instance()->logInfo( tr("Closing tab aborted") );
            return;
        }
    }
    
    if(! getCurrentTab()->_labelsHaveBeenSaved){
        if(QMessageBox::question(this, tr("Labels not saved"), tr("The current labels set has not been saved, do you really want to close this tab ?"),QMessageBox::Yes, QMessageBox::Cancel) != QMessageBox::Yes){
            QArrangementLabellingLogWidget::instance()->logInfo( tr("Closing tab aborted") );
            return;
        }
    }
    
    // delete the tab
    this->_ui->tabWidget->removeTab( currentTabIndex );
    this->_tabs.erase( this->_tabs.begin( ) + currentTabIndex );

    // delete the arrangement
    this->_arrangements.erase( this->_arrangements.begin( ) + currentTabIndex );
    
    // If we have closed the last tab, we forbid the openImage action
    if (! this->_ui->tabWidget->count() || (currentTabIndex == static_cast<unsigned int>(-1)))
        _ui->actionOpenImage->setEnabled(false);
}

void QArrangementLabellingWindow::on_actionZoomIn_triggered( )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    QArrangementLabellingLogWidget::instance()->logDebug( tr("Zoomed in, current scale = ") + QString::number(view->transform().m11()) + tr(" et ") + QString::number(view->transform().m22()) );

    unsigned int currentTabIndex = this->_ui->tabWidget->currentIndex( );
    if (currentTabIndex == static_cast<unsigned int>(-1)) return;
    QArrangementLabellingTabBase* currentTab = this->_tabs[ currentTabIndex ];
    QGraphicsView* view = currentTab->getView( );
    view->scale( 2.0, 2.0 );
}

void QArrangementLabellingWindow::on_actionZoomOut_triggered( )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    QArrangementLabellingLogWidget::instance()->logDebug( tr("Zoomed out, current scale = ") + QString::number(view->transform().m11()) + tr(" et ") + QString::number(view->transform().m22()) );

    unsigned int currentTabIndex = this->_ui->tabWidget->currentIndex( );
    if (currentTabIndex == static_cast<unsigned int>(-1)) return;
    QArrangementLabellingTabBase* currentTab = this->_tabs[ currentTabIndex ];
    QGraphicsView* view = currentTab->getView( );
    view->scale( 0.5, 0.5 );
}

void QArrangementLabellingWindow::on_actionPreferences_triggered( )
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    unsigned int currentTabIndex = this->_ui->tabWidget->currentIndex( );
    if (currentTabIndex == static_cast<unsigned int>(-1))
        return;

    QArrangementLabellingTabBase* currentTab = this->_tabs[ currentTabIndex ];
    CGAL::Qt::ArrangementGraphicsItemBase* agi = currentTab->getArrangementGraphicsItem( );
    QArrangementLabellingGraphicsView* view = currentTab->getView( );
    SplitEdgeCallbackBase* splitEdgeCallback = currentTab->getSplitEdgeCallback( );
    CGAL::Qt::GraphicsViewCurveInputBase* viewCurveInputBase = currentTab->getCurveInputCallback();

    QArrangementLabellingPropertiesDialog* dialog = new QArrangementLabellingPropertiesDialog( this );
    if ( dialog->exec( ) == QDialog::Accepted )
    {
        typedef QArrangementLabellingPropertiesDialog Dialog;
        QColor edgeColor                    = qVariantValue<QColor>(dialog->property(Dialog::EDGE_COLOR_KEY));
        unsigned int edgeWidth              = qVariantValue<unsigned int>(dialog->property(Dialog::EDGE_WIDTH_KEY));
        QColor vertexColor                  = qVariantValue<QColor>(dialog->property(Dialog::VERTEX_COLOR_KEY));
        unsigned int vertexRadius           = qVariantValue<unsigned int>(dialog->property(Dialog::VERTEX_RADIUS_KEY));
        DeleteCurveMode mode                = qVariantValue<DeleteCurveMode>(dialog->property(Dialog::DELETE_CURVE_MODE_KEY));
        unsigned int gridSize               = qVariantValue<unsigned int>(dialog->property(Dialog::GRID_SIZE_KEY));
        QColor gridColor                    = qVariantValue<QColor>(dialog->property(Dialog::GRID_COLOR_KEY));
        unsigned int gridSnappingDistance   = qVariantValue<unsigned int>(dialog->property(Dialog::GRID_SNAPPING_DISTANCE));
        unsigned int vertexSnappingDistance = qVariantValue<unsigned int>(dialog->property(Dialog::VERTEX_SNAPPING_DISTANCE));

        QPen edgesPen(QBrush(edgeColor), edgeWidth);
        QPen verticesPen(QBrush(vertexColor), vertexRadius);
        agi->setEdgesPen( edgesPen );
        agi->setVerticesPen( verticesPen );
        agi->modelChanged( );
        view->setGridSize( gridSize );
        view->setGridColor( gridColor );
        splitEdgeCallback->setColor( edgeColor );

        // Set snapping distances
        ArrangementCurveInputCallback<Pol_arr>* arrangementCurveInputCB = dynamic_cast< ArrangementCurveInputCallback<Pol_arr>* >(viewCurveInputBase);
        if(arrangementCurveInputCB)
        {
            arrangementCurveInputCB->gridSnappingDistance(gridSnappingDistance);
            arrangementCurveInputCB->vertexSnappingDistance(vertexSnappingDistance);

            view->setGridSnappingDistance(gridSnappingDistance);
            view->setVertexSnappingDistance(vertexSnappingDistance);
        }
    }
}

void QArrangementLabellingWindow::on_actionOpenProject_triggered()
{
    QSettings settings(QLABELLING_ORGANIZATION_STRING, QLABELLING_NAME_STRING);
    settings.beginGroup("QLabellingMainWindow");
    QString defaultDirectory = settings.value("defaultProjectDirectory", "").toString();

    QString projectFilename = QFileDialog::getOpenFileName( this, tr( "Open labelling project" ), defaultDirectory, tr("Labelling project files (*.qlb)") );
    if(!projectFilename.isNull() || !projectFilename.isEmpty())
    {
        QFile* file = new QFile(projectFilename);
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            _loggerWidget->logError( tr("Cannot open ") + projectFilename + tr(" labelling project file!") );
            return;
        }
        QXmlStreamReader xml(file);
        settings.setValue("defaultProjectDirectory", QFileInfo(projectFilename).absolutePath());

        QString projectName( tr("undefined") ), projectInputImage, projectLabelsImage, projectArrangement, projectLabelsDefinition;

        while(!xml.atEnd() && !xml.hasError())
        {
            QXmlStreamReader::TokenType token = xml.readNext();
            if(token == QXmlStreamReader::StartDocument)
                continue;
            if(token == QXmlStreamReader::StartElement)
            {
                if(xml.name() == "QLabellingProject")
                {
                    QXmlStreamAttributes attributes = xml.attributes();
                    if(attributes.hasAttribute("name"))
                        projectName = attributes.value("name").toString();
                }
                if(xml.name() == "inputImage")
                {
                    QXmlStreamAttributes attributes = xml.attributes();
                    if(attributes.hasAttribute("path"))
                        projectInputImage = attributes.value("path").toString();
                    else
                    {
                        _loggerWidget->logError( tr("Labelling project ") + projectFilename + tr(" does not contain a valid input image path!") );
                        return;
                    }
                }
                if(xml.name() == "labelsImage")
                {
                    QXmlStreamAttributes attributes = xml.attributes();
                    if(attributes.hasAttribute("path"))
                        projectLabelsImage = attributes.value("path").toString();
                    else
                    {
                        _loggerWidget->logError( tr("Labelling project ") + projectFilename + tr(" does not contain a valid labels image path!") );
                        return;
                    }
                }
                if(xml.name() == "arrangement")
                {
                    QXmlStreamAttributes attributes = xml.attributes();
                    if(attributes.hasAttribute("path"))
                        projectArrangement = attributes.value("path").toString();
                    else
                    {
                        _loggerWidget->logError( tr("Labelling project ") + projectFilename + tr(" does not contain a valid arrangement path!") );
                        return;
                    }
                }
                if(xml.name() == "labelsDefinition")
                {
                    QXmlStreamAttributes attributes = xml.attributes();
                    if(attributes.hasAttribute("path"))
                        projectLabelsDefinition = attributes.value("path").toString();
                    else
                    {
                        _loggerWidget->logError( tr("Labelling project ") + projectFilename + tr(" does not contain a valid labels definition path!") );
                        return;
                    }
                }
            }
        }
        if(xml.hasError())
            _loggerWidget->logError( tr("Error parsing ") + projectFilename + tr(" labelling project file: ") + xml.errorString() );
        xml.clear();

        // If we are here, we just have to check that all files exist.
        if(!QFileInfo(projectInputImage).exists())
            _loggerWidget->logError( tr("Input image ") + projectInputImage + tr(" does not exist!") );
        // TODO: uncomment when labels image will be displayed in GUI
        /*
        if(!QFileInfo(projectLabelsImage).exists())
            _loggerWidget->logError( tr("Labels image ") + projectLabelsImage + tr(" does not exist!") );
            */
        if(!QFileInfo(projectArrangement).exists())
            _loggerWidget->logError( tr("Arrangement ") + projectArrangement + tr(" does not exist!") );
        if(!QFileInfo(projectLabelsDefinition).exists())
            _loggerWidget->logError( tr("Labels definition ") + projectLabelsDefinition + tr(" does not exist!") );

        // If so, we can really open the project
        // The project will be opened in a new tab
        QArrangementLabellingTabBase *projectTab =  makeTab( POLYLINE_TRAITS );
        // TODO: set tab name based on project name ...
        // Load input image
        doLoadImage(projectInputImage);
        // Load arrangement
        openArrFile(projectArrangement);
        // Set labels definition
        QArrangementLabellingWidget::instance()->setLabelsPath( projectLabelsDefinition );
        _ui->tabWidget->setTabText(_ui->tabWidget->count()-1, projectName);

        _loggerWidget->logInfo( tr("Labelling project ") + projectFilename + tr(" loaded") );
    }
    else
        _loggerWidget->logWarning( tr("No 'Labelling project choosed'...") );
}

void QArrangementLabellingWindow::on_actionSaveProject_triggered()
{
    QSettings settings(QLABELLING_ORGANIZATION_STRING, QLABELLING_NAME_STRING);

    if(!getCurrentTab())
    {
        _loggerWidget->logWarning( tr("There is no open tab !") );
        return;
    }

    QArrangementLabellingGraphicsView* tabView = getCurrentTab()->getView();
    if(tabView->imageToLabel().isNull())
    {
        _loggerWidget->logError( tr("No image has been opened. Cannot save such a project!") );
        return;
    }

    QArrangementLabellingSaveProjectDialog spd(tabView->imageToLabelFilename(), this);
    if( spd.exec() )
    {
        QFile file(spd.projectPath());
        if ( !file.open(QIODevice::WriteOnly) )
        {
            QMessageBox::critical(0, tr("Error opening file"), tr("Error opening file") + spd.projectPath());
            return;
        }	
        else
        {
            settings.beginGroup("QLabellingMainWindow");
            settings.setValue("defaultProjectDirectory", QFileInfo(spd.projectPath()).absolutePath());
            settings.endGroup();

            QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
		    xmlWriter->setDevice(&file);
            xmlWriter->setAutoFormatting(true);

		    xmlWriter->writeStartDocument();

            xmlWriter->writeStartElement("QLabellingProject");
            xmlWriter->writeAttribute("name", spd.projectName());

		    xmlWriter->writeStartElement("inputImage");
            xmlWriter->writeAttribute("path", spd.inputImagePath());
            xmlWriter->writeEndElement();

            xmlWriter->writeStartElement("labelsImage");
            xmlWriter->writeAttribute("path", spd.labelsImagePath());
            xmlWriter->writeEndElement();

            xmlWriter->writeStartElement("arrangement");
            xmlWriter->writeAttribute("path", spd.arrangementPath());
            xmlWriter->writeEndElement();

            xmlWriter->writeStartElement("labelsDefinition");
            xmlWriter->writeAttribute("path", spd.labelsDefinitionPath());
            xmlWriter->writeEndElement();

            xmlWriter->writeEndElement(); // QLabellingProject

            xmlWriter->writeEndDocument();

            // Now, write labels image and the arrangement
            doSaveArrangement(spd.arrangementPath());
            saveLabelsImage(tabView->scene(), spd.labelsImagePath());
            
            getCurrentTab()->setArrHasBeenSaved(true);

            _loggerWidget->logInfo( tr("Labelling project saved to ") + spd.projectPath() );
        }
    }
}

bool QArrangementLabellingWindow::on_actionOpenImage_triggered()
{
    QArrangementLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );

    QArrangementLabellingTabBase *currentTab = getCurrentTab();
    if(!currentTab)
    {
        _loggerWidget->logWarning( tr("There is no open tab !") );
        return false;
    }

    const unsigned int TabIndex = this->_ui->tabWidget->currentIndex( );
    if (TabIndex == static_cast<unsigned int>(-1))
    {
        QArrangementLabellingLogWidget::instance()->logError( tr("Unable to add an image : there is no open tab !") );
        if(!currentTab->_imageHasBeenLoaded)
            updateToolBarButtonsEnable(false);
        return false;
    }

    QSettings settings(QLABELLING_ORGANIZATION_STRING, QLABELLING_NAME_STRING);
    
    // Here, it should not be just a warning.
    // 1) If the user changes the image, he should be prompted to save his current project
    // 2) [DONE] If the user chooses a new image, it should not be simply added in the scene. First, the single QPixmap of the scene should be removed, and then,
    // the newly chosen file could be add as a QPixmap to the scene. We must also ensures that all attributes are well set after image change (refactoring may help).
    if(currentTab->_imageHasBeenLoaded)
        QMessageBox::warning( this, tr("Warning"), tr("There is already a loaded image, be sure of what you do.") );

    settings.beginGroup("QLabellingMainWindow");
    QString defaultDirectory = settings.value("defaultDirectory", "").toString();

    bool result = false;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image to label"), defaultDirectory, tr("Image Files (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"));
    if(!fileName.isNull())
    {
        QFileInfo info(fileName);
        settings.setValue("defaultDirectory", info.absolutePath());

        result = doLoadImage(fileName);
    }
    else
    {
        if(!currentTab->_imageHasBeenLoaded)
            updateToolBarButtonsEnable(false);
    }
    settings.endGroup();

    return result;
}

bool QArrangementLabellingWindow::doLoadImage(const QString &fileName)
{
    QArrangementLabellingLogWidget::instance()->logTrace( tr("Opening image ") + fileName );
    QArrangementLabellingTabBase *currentTab = getCurrentTab();
    const unsigned int TabIndex = this->_ui->tabWidget->currentIndex( );

    QGraphicsScene              *currentTabScene = currentTab->getScene();
    QArrangementLabellingGraphicsView *currentTabView  = currentTab->getView();

    bool result = currentTabView->setImageToLabel(fileName, currentTab, getArrangements()[TabIndex]);
    updateToolBarButtonsEnable(result);
    if(result)
    {
        currentTab->_imageHasBeenLoaded = true;
        updateMode( this->_ui->actionInsert );
    }
    else
        currentTab->_imageHasBeenLoaded = false;

    return result;
}

void QArrangementLabellingWindow::updateToolBarButtonsEnable(bool enable)
{
    _ui->actionInsert->setEnabled(enable);
    _ui->actionInsert_horizontal_line->setEnabled(enable);
    _ui->actionInsert_vertical_line->setEnabled(enable);
    _ui->actionDelete->setEnabled(enable);
    _ui->actionPointLocation->setEnabled(enable);
    _ui->actionMerge->setEnabled(enable);
    _ui->actionSplit->setEnabled(enable);
    _ui->actionFill->setEnabled(enable);
//  _   ui->actionFillColor->setEnabled(enable);
    _ui->actionDrag->setEnabled(enable);
    _ui->actionZoomIn->setEnabled(enable);
    _ui->actionZoomOut->setEnabled(enable);
    _ui->actionSnapMode->setEnabled(enable);
    _ui->actionGridSnapMode->setEnabled(enable);
    _ui->actionClean->setEnabled(enable);
    
    // Enabling/Disabling the label list too
    QArrangementLabellingWidget::instance()->setEnabledAllLabelButtons(enable);
}


// Clean the arrangement
void QArrangementLabellingWindow::on_actionClean_triggered()
{
    int tabIndex = this->_ui->tabWidget->currentIndex( );
    if ( tabIndex == -1 )
    {
        QMessageBox::information( this, tr("Oops"), tr("Create a new tab first") );
        return;
    }
    
    CGAL::Object arr = this->_arrangements[ tabIndex ];
    Pol_arr* pol;
    
    if ( CGAL::assign( pol, arr ) )
    {
        int index;
        
        // Suppression des vertices isolés
        Pol_arr::Vertex_iterator vit, vnext = pol->vertices_begin();
        for (vit = vnext++, index=0 ; vit != pol->vertices_end(); vit = vnext++, ++index)
        {
            // This should theorically never happen
            if(vit->is_isolated())
            {
                QArrangementLabellingLogWidget::instance()->logTrace(tr("Removing isolated vertex %n.","",index));
                pol->remove_isolated_vertex(vit);
                continue;
            }
        }
        
        // Labelling unbounded face
        pol->unbounded_face()->data().set_label("Unbounded");
        
        // Suppression des antennes, et des HE en dehors de l'image
         // On a besoin de connaître la taille de l'image !
         QRect rectIm = this->getCurrentTab()->getView()->imageToLabel().rect();
         
        Pol_arr::Edge_iterator eit, enext;
        for (enext = pol->edges_begin(), eit = enext, enext++, index=0 ; eit != pol->edges_end(); eit = enext, enext++, ++index)
        {
            // Antenna test
            if(eit->twin()->face() == eit->face())
            {
                QArrangementLabellingLogWidget::instance()->logTrace(tr("Removing antenna halfedge %n.","",index));
                pol->remove_edge(eit);
                continue;
            }
            
            // Source or target out-of-pixmap ?
            if(CGAL::to_double(eit->source()->point().x()) < rectIm.left()
            || CGAL::to_double(eit->source()->point().x()) > rectIm.right()+1
            || CGAL::to_double(eit->source()->point().y()) > rectIm.bottom()+1
            || CGAL::to_double(eit->source()->point().y()) < rectIm.top() )
            {
                QArrangementLabellingLogWidget::instance()->logTrace(tr("Source out-of-pixmap : removing halfedge %n.","",index));
                pol->remove_edge(eit);
                continue;
            }
            if(CGAL::to_double(eit->target()->point().x()) < rectIm.left()
            || CGAL::to_double(eit->target()->point().x()) > rectIm.right()+1
            || CGAL::to_double(eit->target()->point().y()) > rectIm.bottom()+1
            || CGAL::to_double(eit->target()->point().y()) < rectIm.top() )
            {
                QArrangementLabellingLogWidget::instance()->logTrace(tr("Target out-of-pixmap : removing halfedge %n.","",index));
                pol->remove_edge(eit);
                continue;
            }
        }
            
        for (enext = pol->edges_begin(), eit = enext, enext++, index=0 ; eit != pol->edges_end(); eit = enext, enext++, ++index)
        {
            // Twin face has the same label ?
            if(eit->twin()->face()->data().label() == eit->face()->data().label()){
                QArrangementLabellingLogWidget::instance()->logTrace(tr("Twin face has the same label (") + eit->face()->data().label() + tr(") : merging faces by removing halfedge %n.","",index));
                pol->remove_edge(eit);
                continue;
            }
        }
        
        // Removing all vertices whose degree is 2 (with mergeable edges) - meaning removing all vertices which could be removed...
        vnext = pol->vertices_begin();
        for (vit = vnext++, index=0 ; vit != pol->vertices_end(); vit = vnext++, ++index)
        {
            if(vit->degree() == 2)
            {
                Pol_arr::Halfedge_around_vertex_circulator first, curr;
                first = curr = vit->incident_halfedges();
                curr++;
                if(pol->are_mergeable(first,curr))
                {
                    QArrangementLabellingLogWidget::instance()->logTrace(tr("Merging edges."));
                    pol->merge_edge(first,curr);
                }
            }
            
        }
    }
    else
    {
        QArrangementLabellingLogWidget::instance()->logError(tr("[Cleaning] Parsing arrangement failed..."));
    }

    this->_tabs[ tabIndex ]->getScene()->update( );
}

// Save the label-colored image
void QArrangementLabellingWindow::saveLabelsImage(QGraphicsScene *scene, const QString& filename)
{
    if(!scene)
        return;    

    QRect rectIm = this->getCurrentTab()->getView()->imageToLabel().rect();
    QImage image(rectIm.width(), rectIm.height(), QImage::Format_RGB32);
    QPainter painter(&image);

    CGAL::Qt::ArrangementGraphicsItem<Pol_arr>* agi = (CGAL::Qt::ArrangementGraphicsItem<Pol_arr>*)getCurrentTab()->getArrangementGraphicsItem();
    agi->paintFaces(true, &painter);

    if(!image.save(filename))
        QArrangementLabellingLogWidget::instance()->logError( tr("Error saving the label image...") );
}

