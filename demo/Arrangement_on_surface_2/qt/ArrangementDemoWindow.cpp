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
#include "NewTabDialog.h"
#include "OverlayDialog.h"
#include "ArrangementDemoPropertiesDialog.h"
#include "ArrangementDemoTab.h"
#include "DeleteCurveMode.h"
#include "ArrangementGraphicsItem.h"

#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>

#include <CGAL/IO/Arr_with_history_iostream.h>
#include <CGAL/IO/Arr_text_formatter.h>
#include <CGAL/IO/Arr_with_history_text_formatter.h>

#ifdef _WINDOWS
    // for isinf
    #include <boost/math/special_functions/fpclassify.hpp>
#endif // _WINDOWS

ArrangementDemoWindow::ArrangementDemoWindow(QWidget* parent) :
  CGAL::Qt::DemosMainWindow( parent ),
  lastTabIndex(static_cast<unsigned int>(-1)),
  ui( new Ui::ArrangementDemoWindow )
{
  this->setupUi( );

  // set up the demo window
  // ArrangementDemoTabBase* demoTab =
  this->makeTab( SEGMENT_TRAITS ); 
  this->setupStatusBar( );
  this->setupOptionsMenu( );
  this->addAboutDemo( ":/help/about.html" );
  this->addAboutCGAL( );

  // set up callbacks
  QObject::connect( this->modeGroup, SIGNAL( triggered( QAction* ) ),
                    this, SLOT( updateMode( QAction* ) ) );
  QObject::connect( this->envelopeGroup, SIGNAL( triggered( QAction* ) ),
                    this, SLOT( updateEnvelope( QAction* ) ) );
  QObject::connect( this->snapGroup, SIGNAL( triggered( QAction* ) ),
                    this, SLOT( updateSnapping( QAction* ) ) );
}

ArrangementDemoTabBase* ArrangementDemoWindow::makeTab( TraitsType tt )
{
  static int tabLabelCounter = 1;
  QString tabLabel;

  ArrangementDemoTabBase* demoTab;
  Seg_arr* seg_arr;
  Pol_arr* pol_arr;
  Lin_arr* lin_arr;
  CGAL::Object arr;

  switch ( tt )
  {
   default:
   case SEGMENT_TRAITS:
    seg_arr = new Seg_arr;
    demoTab = new ArrangementDemoTab< Seg_arr >( seg_arr, 0 );
    arr = CGAL::make_object( seg_arr );
    tabLabel = QString( "%1 - Segment" ).arg( tabLabelCounter++ );
    break;
   case POLYLINE_TRAITS:
    pol_arr = new Pol_arr;
    demoTab = new ArrangementDemoTab< Pol_arr >( pol_arr, 0 );
    arr = CGAL::make_object( pol_arr );
    tabLabel = QString( "%1 - Polyline" ).arg( tabLabelCounter++ );
    break;
   case LINEAR_TRAITS:
    lin_arr = new Lin_arr;
    demoTab = new ArrangementDemoTab< Lin_arr >( lin_arr, 0 );
    arr = CGAL::make_object( lin_arr );
    tabLabel = QString( "%1 - Linear" ).arg( tabLabelCounter++ );
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
  if (tabIndex > this->tabs.size()) return NULL;
  return this->tabs[tabIndex];
}

ArrangementDemoTabBase* ArrangementDemoWindow::getCurrentTab( ) const
{
  int currentIndex = this->ui->tabWidget->currentIndex( );
  if ( currentIndex == -1 )
    return NULL;

  ArrangementDemoTabBase* res = this->tabs[ currentIndex ];
  return res;
}

std::vector< QString > ArrangementDemoWindow::getTabLabels( ) const
{
  std::vector< QString > res;
  for ( int i = 0; i < this->ui->tabWidget->count( ); ++i )
  {
    res.push_back( this->ui->tabWidget->tabText( i ) );
  }
  return res;
}

std::vector< CGAL::Object > ArrangementDemoWindow::getArrangements( ) const
{
  std::vector< CGAL::Object > res;
  for ( unsigned int i = 0; i < this->arrangements.size( ); ++i )
  {
    res.push_back( this->arrangements[ i ] );
  }
  return res;
}

void ArrangementDemoWindow::setupUi( )
{
  this->ui->setupUi( this );

  this->modeGroup = new QActionGroup( this );
  this->modeGroup->addAction( this->ui->actionDrag );
  this->modeGroup->addAction( this->ui->actionInsert );
  this->modeGroup->addAction( this->ui->actionDelete );
  this->modeGroup->addAction( this->ui->actionPointLocation );
  this->modeGroup->addAction( this->ui->actionRayShootingUp );
  this->modeGroup->addAction( this->ui->actionRayShootingDown );
  this->modeGroup->addAction( this->ui->actionMerge );
  this->modeGroup->addAction( this->ui->actionSplit );
  this->modeGroup->addAction( this->ui->actionFill );
  this->activeModes.push_back( this->ui->actionInsert );

  this->envelopeGroup = new QActionGroup( this );
  this->envelopeGroup->addAction( this->ui->actionLowerEnvelope );
  this->envelopeGroup->addAction( this->ui->actionUpperEnvelope );
  this->envelopeGroup->setExclusive( false );

  this->snapGroup = new QActionGroup( this );
  this->snapGroup->addAction( this->ui->actionSnapMode );
  this->snapGroup->addAction( this->ui->actionGridSnapMode );
  this->snapGroup->setExclusive( false );
  this->ui->actionGridSnapMode->setEnabled( false );

  this->updateFillColorSwatch( );
}

void ArrangementDemoWindow::updateMode( QAction* newMode )
{
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

  // hook up the new active mode
  if ( newMode == this->ui->actionInsert )
  {
    activeScene->installEventFilter( activeTab->getCurveInputCallback( ) );
  }
  else if ( newMode == this->ui->actionDrag )
  {
    activeView->setDragMode( QGraphicsView::ScrollHandDrag );
  }
  else if ( newMode == this->ui->actionDelete )
  {
    activeScene->installEventFilter( activeTab->getDeleteCurveCallback( ) );
  }
  else if ( newMode == this->ui->actionPointLocation )
  {
    activeScene->installEventFilter( activeTab->getPointLocationCallback( ) );
  }
  else if ( newMode == this->ui->actionRayShootingUp )
  {
    // -y is up for Qt, so we shoot down
    activeTab->getVerticalRayShootCallback( )->setShootingUp( true );
    activeScene->installEventFilter( activeTab->getVerticalRayShootCallback());
  }
  else if ( newMode == this->ui->actionRayShootingDown )
  {
    // the bottom of the viewport for Qt is +y, so we shoot up
    activeTab->getVerticalRayShootCallback( )->setShootingUp( false );
    activeScene->installEventFilter( activeTab->getVerticalRayShootCallback());
  }
  else if ( newMode == this->ui->actionMerge )
  {
    activeScene->installEventFilter( activeTab->getMergeEdgeCallback( ) );
  }
  else if ( newMode == this->ui->actionSplit )
  {
    activeScene->installEventFilter( activeTab->getSplitEdgeCallback( ) );
  }
  else if ( newMode == this->ui->actionFill )
  {
    activeScene->installEventFilter( activeTab->getFillFaceCallback( ) );
  }
  this->updateFillColorSwatch( );
}

void ArrangementDemoWindow::resetCallbackState( unsigned int tabIndex )
{
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
  else if ( activeMode == this->ui->actionRayShootingUp )
  {
    activeTab->getVerticalRayShootCallback( )->reset( );
  }
  else if ( activeMode == this->ui->actionRayShootingDown )
  {
    activeTab->getVerticalRayShootCallback( )->reset( );
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
  activeScene->removeEventFilter( activeTab->getVerticalRayShootCallback( ) );
  activeScene->removeEventFilter( activeTab->getMergeEdgeCallback( ) );
  activeScene->removeEventFilter( activeTab->getSplitEdgeCallback( ) );
  activeScene->removeEventFilter( activeTab->getVerticalRayShootCallback( ) );
  activeScene->removeEventFilter( activeTab->getFillFaceCallback( ) );
}

void ArrangementDemoWindow::updateFillColorSwatch( )
{
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
  Seg_arr* seg;
  Pol_arr* pol;
  if ( CGAL::assign( seg, arr ) )
  {
    typedef CGAL::Arr_text_formatter<Seg_arr>           Seg_text_formatter;
    typedef CGAL::Arr_with_history_text_formatter<Seg_text_formatter>
      ArrFormatter;
    typedef ArrangementDemoTab<Seg_arr>                 TabType;

    ArrFormatter arrFormatter;
    CGAL::read( *seg, ifs, arrFormatter );
    this->arrangements[ index ] = CGAL::make_object( seg );
    TabType* tab = static_cast< TabType* >( this->tabs[ index ] );
    tab->setArrangement( seg );
  }
  else if ( CGAL::assign( pol, arr ) )
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
  Seg_arr* seg;
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
        points.push_back (Arr_pol_point_2(NT(ix),NT(iy)));
      }

      Arr_pol_2 curve (points.begin(), points.end());
      pol_list.push_back(curve);
    }
    CGAL::insert(*pol, pol_list.begin(), pol_list.end());

    typedef ArrangementDemoTab< Pol_arr > TabType;
    TabType* tab = static_cast< TabType* >( this->tabs[ index ] );
    tab->setArrangement( pol );
  }
  else if ( CGAL::assign( seg, arr ) )
  {
    seg->clear( );

    int count;
    inputFile >> count;
    int i;
    std::list<Arr_seg_2> seg_list;
    for (i = 0; i < count; i++)
    {
      NT x0, y0, x1, y1;
      inputFile >> x0 >> y0 >> x1 >> y1;

      Arr_seg_point_2 p1(x0, y0);
      Arr_seg_point_2 p2(x1, y1);

      Arr_seg_2 curve(p1, p2);

      seg_list.push_back(curve);
    }

    CGAL::insert(*(seg), seg_list.begin(), seg_list.end());

    typedef ArrangementDemoTab< Seg_arr > TabType;
    TabType* tab = static_cast< TabType* >( this->tabs[ index ] );
    tab->setArrangement( seg );
  }

  inputFile.close();
}

void ArrangementDemoWindow::updateEnvelope( QAction* newMode )
{
  if ( this->ui->tabWidget->currentIndex( ) == -1 ) return;
  ArrangementDemoTabBase* activeTab =
    this->tabs[ this->ui->tabWidget->currentIndex( ) ];
  // QGraphicsScene* activeScene = activeTab->getScene( );
  // QGraphicsView* activeView = activeTab->getView( );

  bool show = newMode->isChecked( );
  if ( newMode == this->ui->actionLowerEnvelope )
  {
    activeTab->getEnvelopeCallback( )->showLowerEnvelope( show );
  }
  else if ( newMode == this->ui->actionUpperEnvelope )
  {
    activeTab->getEnvelopeCallback( )->showUpperEnvelope( show );
  }
}

void ArrangementDemoWindow::updateSnapping( QAction* newMode )
{
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
  Seg_arr* seg;
  Pol_arr* pol;
  if ( CGAL::assign( seg, arr ) )
  {
    typedef CGAL::Arr_text_formatter<Seg_arr>           Seg_text_formatter;
    typedef CGAL::Arr_with_history_text_formatter<Seg_text_formatter>
      ArrFormatter;
    ArrFormatter                                        arrFormatter;
    CGAL::write( *seg, ofs, arrFormatter );
  }
  else if ( CGAL::assign( pol, arr ) )
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
  qApp->exit( );
}

void ArrangementDemoWindow::on_actionNewTab_triggered( )
{
  NewTabDialog* newTabDialog = new NewTabDialog;
  if ( newTabDialog->exec( ) == QDialog::Accepted )
  {
    int id = newTabDialog->checkedId( );
    if ( id == SEGMENT_TRAITS )
    {
      this->makeTab( SEGMENT_TRAITS );
    }
    else if ( id == POLYLINE_TRAITS )
    {
      this->makeTab( POLYLINE_TRAITS );
    }
    else if ( id == LINEAR_TRAITS )
    {
      this->makeTab( LINEAR_TRAITS );
    }
    else
    {
      std::cout << "Sorry, this trait is not yet supported" << std::endl;
    }
  }
  delete newTabDialog;
}

void ArrangementDemoWindow::on_tabWidget_currentChanged( )
{
  // std::cout << "Tab changed" << std::endl;
  // disable the callback for the previously active tab
  this->resetCallbackState( this->lastTabIndex );
  this->removeCallback( this->lastTabIndex );
  this->lastTabIndex = this->ui->tabWidget->currentIndex( );

  this->updateMode( this->modeGroup->checkedAction( ) );

  CGAL::Object arr;
  if ( this->ui->tabWidget->currentIndex( ) != -1 )
    arr = this->arrangements[ this->ui->tabWidget->currentIndex( ) ];

  /*
  Lin_arr* lin;
  if ( CGAL::assign( lin, arr ) )
  {
    this->ui->actionCurveRay->setVisible( true );
    this->ui->actionCurveLine->setVisible( true );
  }
  else
  { // segment or polyline
    this->ui->actionConicSegment->setChecked( true );

    this->ui->actionCurveRay->setVisible( false );
    this->ui->actionCurveLine->setVisible( false );
  }
  */
}

void ArrangementDemoWindow::on_actionOverlay_triggered( )
{
  OverlayDialog* overlayDialog = new OverlayDialog( this );
  if ( overlayDialog->exec( ) == QDialog::Accepted )
  {
    std::vector< CGAL::Object > arrs = overlayDialog->selectedArrangements( );
    if ( arrs.size( ) == 2 )
    {
      Seg_arr* seg_arr;
      Seg_arr* seg_arr2;
      Pol_arr* pol_arr;
      Pol_arr* pol_arr2;
      Lin_arr* lin_arr;
      Lin_arr* lin_arr2;
      if ( CGAL::assign( seg_arr, arrs[ 0 ] ) &&
           CGAL::assign( seg_arr2, arrs[ 1 ] ) )
      {
        this->makeOverlayTab( seg_arr, seg_arr2 );
      }
      if ( CGAL::assign( pol_arr, arrs[ 0 ] ) &&
           CGAL::assign( pol_arr2, arrs[ 1 ] ) )
      {
        this->makeOverlayTab( pol_arr, pol_arr2 );
      }
      if ( CGAL::assign( lin_arr, arrs[ 0 ] ) &&
           CGAL::assign( lin_arr2, arrs[ 1 ] ) )
      {
        this->makeOverlayTab( lin_arr, lin_arr2 );
      }
    }
  }
  delete overlayDialog;
}

void ArrangementDemoWindow::on_actionCloseTab_triggered( )
{
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
  unsigned int currentTabIndex = this->ui->tabWidget->currentIndex( );
  if (currentTabIndex == static_cast<unsigned int>(-1)) return;
  ArrangementDemoTabBase* currentTab = this->tabs[ currentTabIndex ];
  QGraphicsView* view = currentTab->getView( );
  view->scale( 2.0, 2.0 );
}

void ArrangementDemoWindow::on_actionZoomOut_triggered( )
{
  unsigned int currentTabIndex = this->ui->tabWidget->currentIndex( );
  if (currentTabIndex == static_cast<unsigned int>(-1)) return;
  ArrangementDemoTabBase* currentTab = this->tabs[ currentTabIndex ];
  QGraphicsView* view = currentTab->getView( );
  view->scale( 0.5, 0.5 );
}

void ArrangementDemoWindow::on_actionPreferences_triggered( )
{
  unsigned int currentTabIndex = this->ui->tabWidget->currentIndex( );
  if (currentTabIndex == static_cast<unsigned int>(-1)) return;
  ArrangementDemoTabBase* currentTab = this->tabs[ currentTabIndex ];
  CGAL::Qt::ArrangementGraphicsItemBase* agi =
    currentTab->getArrangementGraphicsItem( );
  ArrangementDemoGraphicsView* view = currentTab->getView( );
  EnvelopeCallbackBase* envelopeCallback = currentTab->getEnvelopeCallback( );
  VerticalRayShootCallbackBase* verticalRayShootCallback =
    currentTab->getVerticalRayShootCallback( );
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
    QColor envelopeEdgeColor =
      qVariantValue<QColor>(dialog->property(Dialog::ENVELOPE_EDGE_COLOR_KEY));
    unsigned int envelopeEdgeWidth =
      qVariantValue<unsigned int>(dialog->property(Dialog::
                                                   ENVELOPE_EDGE_WIDTH_KEY));
    QColor envelopeVertexColor =
      qVariantValue<QColor>(dialog->property(Dialog::
                                             ENVELOPE_VERTEX_COLOR_KEY));
    unsigned int envelopeVertexRadius =
      qVariantValue<unsigned int>(dialog->property(Dialog::
                                                   ENVELOPE_VERTEX_RADIUS_KEY));
    QColor verticalRayEdgeColor =
      qVariantValue<QColor>(dialog->property(Dialog::
                                             VERTICAL_RAY_EDGE_COLOR_KEY));
    unsigned int verticalRayEdgeWidth =
      qVariantValue<unsigned int>(dialog->property
                                  (Dialog::VERTICAL_RAY_EDGE_WIDTH_KEY));
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
    envelopeCallback->setEnvelopeEdgeColor( envelopeEdgeColor );
    envelopeCallback->setEnvelopeEdgeWidth( envelopeEdgeWidth );
    envelopeCallback->setEnvelopeVertexColor( envelopeVertexColor );
    envelopeCallback->setEnvelopeVertexRadius( envelopeVertexRadius );
    verticalRayShootCallback->setEdgeColor( verticalRayEdgeColor );
    verticalRayShootCallback->setEdgeWidth( verticalRayEdgeWidth );
    splitEdgeCallback->setColor( edgeColor );
  }
}

void ArrangementDemoWindow::on_actionFillColor_triggered( )
{
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

