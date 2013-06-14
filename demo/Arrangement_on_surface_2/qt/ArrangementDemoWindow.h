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

#ifndef ARRANGEMENT_DEMO_WINDOW_H
#define ARRANGEMENT_DEMO_WINDOW_H

#include "ArrangementGraphicsItem.h"
#include "DeleteCurveCallback.h"
#include "PointLocationCallback.h"
#include "MergeEdgeCallback.h"
#include "SplitEdgeCallback.h"
#include "ArrangementDemoTab.h"

#include <CGAL/Arr_default_overlay_traits.h>
#include <CGAL/Qt/DemosMainWindow.h>
#include <CGAL/IO/pixmaps/hand.xpm>

#include "ui_ArrangementDemoWindow.h"

#include <QGraphicsView>

#include <QSettings>

namespace Ui { class ArrangementDemoWindow; }

class QActionGroup;

class QLabellingLogWidget;
class QLabellingWidget;

class ArrangementDemoWindow : public CGAL::Qt::DemosMainWindow
{
    Q_OBJECT
public:
    typedef enum TraitsType {
        POLYLINE_TRAITS
    } TraitsType;
    
    ArrangementDemoWindow(QWidget* parent = 0);
    ~ArrangementDemoWindow() {}

    ArrangementDemoTabBase* makeTab( TraitsType tt );
    ArrangementDemoTabBase* getTab( unsigned int tabIndex ) const;
    ArrangementDemoTabBase* getCurrentTab( ) const;

    std::vector< QString > getTabLabels( ) const;
    std::vector< CGAL::Object > getArrangements( ) const;

    template < class ArrType >
    void makeOverlayTab( ArrType* arr1, ArrType* arr2 );
    
public slots:
    void updateMode( QAction* a );
    void updateSnapping( QAction* a );
    void on_actionNewTab_triggered( );
    void on_actionSaveAs_triggered( );
    void on_actionOpen_triggered( );
    bool on_actionOpenImage_triggered();
    void on_actionQuit_triggered( );
    void on_tabWidget_currentChanged( );
    void on_actionOverlay_triggered( );
    void on_actionClean_triggered( );
    void on_actionCloseTab_triggered( );
    void on_actionZoomIn_triggered( );
    void on_actionZoomOut_triggered( );
    void on_actionPreferences_triggered( );
//     void on_actionFillColor_triggered( );
    void labelChanged();

signals:
    void modelChanged( );

protected:
    void setupUi( );
    void resetCallbackState( unsigned int tabIndex );
    void removeCallback( unsigned int tabIndex );
//     void updateFillColorSwatch( );

    void openArrFile( QString filename );
    void openDatFile( QString filename );

    std::vector< ArrangementDemoTabBase* > tabs;
    std::vector< CGAL::Object > arrangements;
    std::vector< QAction* > activeModes; // for the current tab; always size 1
    unsigned int lastTabIndex;

    Ui::ArrangementDemoWindow* ui;
    QActionGroup* modeGroup;
    QActionGroup* snapGroup;

    QLabellingLogWidget *_loggerWidget; // Le logger
    QLabellingWidget *_labellingWidget; // Le widget de choix des labels + des modes
    
    // Image chargée depuis un fichier
    QPixmap *_labelsPixmap;

    void updateToolBarButtonsEnable(bool enable);
};

template < class ArrType >
void
ArrangementDemoWindow::
makeOverlayTab( ArrType* arr1, ArrType* arr2 )
{
    QString tabLabel = QString( "Overlay Tab" );

    ArrangementDemoTabBase* demoTab;
    ArrType* overlayArr = new ArrType;
    CGAL::Arr_default_overlay_traits< ArrType > defaultTraits;

    CGAL::overlay( *arr1, *arr2, *overlayArr, defaultTraits );

    demoTab = new ArrangementDemoTab< ArrType >( overlayArr, 0 );
    this->arrangements.push_back( CGAL::make_object( overlayArr ) );
    this->tabs.push_back( demoTab );

    QGraphicsView* view = demoTab->getView( );
    this->addNavigation( view );
    this->ui->tabWidget->addTab( demoTab, tabLabel );
    this->lastTabIndex = this->ui->tabWidget->currentIndex( );
}

#endif // ARRANGEMENT_DEMO_WINDOW_H
