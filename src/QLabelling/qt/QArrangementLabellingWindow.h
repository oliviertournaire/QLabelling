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

#ifndef __QARRANGEMENT_LABELLING_WINDOW_H__
#define __QARRANGEMENT_LABELLING_WINDOW_H__

#include "ArrangementGraphicsItem.h"
#include "QArrangementLabellingTab.h"

#include <CGAL/Arr_default_overlay_traits.h>
#include <CGAL/Qt/DemosMainWindow.h>
#include <CGAL/IO/pixmaps/hand.xpm>

#include <QGraphicsView>

#include <QSettings>

#include "ui_QArrangementLabellingWindow.h"
#include "QArrangementLabellingVanishingPointsWidget.h"

class QActionGroup;

class QArrangementLabellingLogWidget;
class QArrangementLabellingWidget;
class QArrangementLabellingVanishingPointsWidget;

class QArrangementLabellingWindow : public CGAL::Qt::DemosMainWindow
{
    Q_OBJECT
public:
    typedef enum TraitsType {
        POLYLINE_TRAITS
    } TraitsType;
    
    QArrangementLabellingWindow(QWidget* parent = 0);
    ~QArrangementLabellingWindow() {}

    QArrangementLabellingTabBase* makeTab( TraitsType tt );
    QArrangementLabellingTabBase* getTab( unsigned int tabIndex ) const;
    QArrangementLabellingTabBase* getCurrentTab( ) const;

    std::vector< QString > getTabLabels( ) const;
    std::vector< CGAL::Object > getArrangements( ) const;

    template < class ArrType >
    void makeOverlayTab( ArrType* arr1, ArrType* arr2 );
    QActionGroup* _modeGroup;
    
public slots:
    void updateMode( QAction* a );
    void updateSnapping( QAction* a );
    void on_actionNewTab_triggered( );
    void on_actionSaveAs_triggered( );
    void on_actionOpenProject_triggered( );
    void on_actionSaveProject_triggered( );
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
    void switchVanishingmode();

    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

    void saveLabelsImage(QGraphicsScene* scene, const QString& filename);
    void labelChanged();

signals:
    void modelChanged( );

protected:
    void setupUi( );
    void doSaveArrangement(const QString& filename);
    void resetCallbackState( unsigned int tabIndex );
    void removeCallback( unsigned int tabIndex );

    void openArrFile( QString filename );
    void openDatFile( QString filename );

    std::vector< QArrangementLabellingTabBase* > _tabs;
    std::vector< CGAL::Object > _arrangements;
    std::vector< QAction* > _activeModes; // for the current tab; always size 1
    unsigned int _lastTabIndex;

    Ui::QArrangementLabellingWindow* _ui;

    QActionGroup* _snapGroup;

    QArrangementLabellingLogWidget *_loggerWidget; // Le logger
    QArrangementLabellingWidget *_labellingWidget; // Le widget de choix des labels + des modes
    QArrangementLabellingVanishingPointsWidget *_vanishingWidget;

    bool doLoadImage(const QString &fileName);
    void updateToolBarButtonsEnable(bool enable);
};

template < class ArrType >
void QArrangementLabellingWindow::makeOverlayTab( ArrType* arr1, ArrType* arr2 )
{
    QString tabLabel = tr( "Overlay Tab" );

    QArrangementLabellingTabBase* demoTab;
    ArrType* overlayArr = new ArrType;
    CGAL::Arr_default_overlay_traits< ArrType > defaultTraits;

    CGAL::overlay( *arr1, *arr2, *overlayArr, defaultTraits );

    demoTab = new QArrangementLabellingTab< ArrType >( overlayArr, 0 );
    this->_arrangements.push_back( CGAL::make_object( overlayArr ) );
    this->_tabs.push_back( demoTab );

    QGraphicsView* view = demoTab->getView( );
    this->addNavigation( view );
    this->_ui->tabWidget->addTab( demoTab, tabLabel );
    this->_lastTabIndex = this->_ui->tabWidget->currentIndex( );
}

#endif // __QARRANGEMENT_LABELLING_WINDOW_H__
