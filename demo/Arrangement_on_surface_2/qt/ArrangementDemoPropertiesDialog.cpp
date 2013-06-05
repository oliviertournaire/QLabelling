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

#include "ArrangementDemoPropertiesDialog.h"
#include "ui_ArrangementDemoPropertiesDialog.h"
#include "ArrangementDemoWindow.h"
#include "PropertyValueDelegate.h"
#include "DeleteCurveMode.h"
#include "ArrangementDemoGraphicsView.h"

ArrangementDemoPropertiesDialog::
ArrangementDemoPropertiesDialog( ArrangementDemoWindow* parent_,
                                 Qt::WindowFlags f ) :
    QDialog( parent_, f ),
    parent( parent_ ),
    ui( new Ui::ArrangementDemoPropertiesDialog )
{
    this->setupUi( );
}

QVariant ArrangementDemoPropertiesDialog::property( int index )
{
    // return invalid variant if something goes wrong
    QVariant res;
    if ( index < 0 || index >= this->ui->tableWidget->rowCount( ) )
    {
        return res;
    }

    QTableWidgetItem* item = this->ui->tableWidget->item( index, 0 );
    if ( item == 0 )
    {
        return res;
    }

    // return user data, if it is set
    QVariant myData = item->data( Qt::UserRole );
    if ( qVariantCanConvert< QColor >( myData ) ||
         qVariantCanConvert< DeleteCurveMode >( myData ) )
    {
        return myData;
    }
    else
    {
        res = item->data( Qt::DisplayRole );
    }

    return res;
}

void ArrangementDemoPropertiesDialog::setupUi( )
{
    this->ui->setupUi( this );
    PropertyValueDelegate* myDelegate = new PropertyValueDelegate;
    this->ui->tableWidget->setItemDelegate( myDelegate );

    // populate the table widget with items
    QTableWidgetItem* edgeColorItem = new QTableWidgetItem;
    QTableWidgetItem* vertexColorItem = new QTableWidgetItem;
    QTableWidgetItem* edgeWidthItem = new QTableWidgetItem;
    QTableWidgetItem* vertexRadiusItem = new QTableWidgetItem;
    QTableWidgetItem* deleteCurveModeItem = new QTableWidgetItem;
    QTableWidgetItem* gridSizeItem = new QTableWidgetItem;
    QTableWidgetItem* gridColorItem = new QTableWidgetItem;

    this->ui->tableWidget->setItem( int(EDGE_COLOR_KEY), 0, edgeColorItem );
    this->ui->tableWidget->setItem( int(VERTEX_COLOR_KEY), 0, vertexColorItem );
    this->ui->tableWidget->setItem( int(EDGE_WIDTH_KEY), 0, edgeWidthItem );
    this->ui->tableWidget->setItem( int(VERTEX_RADIUS_KEY), 0, vertexRadiusItem );
    this->ui->tableWidget->setItem( int(DELETE_CURVE_MODE_KEY), 0, deleteCurveModeItem );
    this->ui->tableWidget->setItem( int(GRID_SIZE_KEY), 0, gridSizeItem );
    this->ui->tableWidget->setItem( int(GRID_COLOR_KEY), 0, gridColorItem );

    // fill in the items with data
    this->updateUi( );
}

void ArrangementDemoPropertiesDialog::updateUi( )
{
    if ( this->parent == NULL )
    {
        return;
    }
    ArrangementDemoTabBase* currentTab = this->parent->getCurrentTab( );
    if ( currentTab == NULL )
    {
        return;
    }
    CGAL::Qt::ArrangementGraphicsItemBase* agi = currentTab->getArrangementGraphicsItem( );
    if ( agi == NULL )
    {
        return;
    }

    ArrangementDemoGraphicsView* view = currentTab->getView( );
    QPen vertexPen = agi->getVerticesPen( );
    QPen edgePen = agi->getEdgesPen( );
    QBrush vertexPenBrush = vertexPen.brush( );
    QBrush edgePenBrush = edgePen.brush( );
    QColor vertexColor = vertexPenBrush.color( );
    QColor edgeColor = edgePenBrush.color( );
    unsigned int edgeWidth = edgePen.width( );
    unsigned int vertexRadius = vertexPen.width( );
    unsigned int gridSize = view->getGridSize( );
    QColor gridColor = view->getGridColor( );
    
    QTableWidgetItem* edgeColorItem =
            this->ui->tableWidget->item( int(EDGE_COLOR_KEY), 0 );
    QTableWidgetItem* edgeWidthItem =
            this->ui->tableWidget->item( int(EDGE_WIDTH_KEY), 0 );
    QTableWidgetItem* vertexColorItem =
            this->ui->tableWidget->item( int(VERTEX_COLOR_KEY), 0 );
    QTableWidgetItem* vertexRadiusItem =
            this->ui->tableWidget->item( int(VERTEX_RADIUS_KEY), 0 );
    QTableWidgetItem* deleteCurveModeItem =
            this->ui->tableWidget->item( int(DELETE_CURVE_MODE_KEY), 0 );
    QTableWidgetItem* gridSizeItem =
            this->ui->tableWidget->item( int( GRID_SIZE_KEY ), 0 );
    QTableWidgetItem* gridColorItem =
            this->ui->tableWidget->item( int( GRID_COLOR_KEY ), 0 );

    // arrangement properties
    edgeColorItem->setData( Qt::DisplayRole, edgeColor );
    edgeColorItem->setData( Qt::DecorationRole, edgeColor );
    edgeColorItem->setData( Qt::UserRole, QVariant::fromValue( edgeColor ) );

    edgeWidthItem->setData( Qt::DisplayRole, edgeWidth );

    vertexColorItem->setData( Qt::DisplayRole, vertexColor );
    vertexColorItem->setData( Qt::DecorationRole, vertexColor );
    vertexColorItem->setData( Qt::UserRole, QVariant::fromValue( vertexColor ) );

    vertexRadiusItem->setData( Qt::DisplayRole, vertexRadius );

    // delete curve properties
    DeleteCurveMode deleteCurveMode;
    deleteCurveModeItem->setData( Qt::DisplayRole,
                                  DeleteCurveMode::ToString( deleteCurveMode ) );
    deleteCurveModeItem->setData( Qt::UserRole,
                                  QVariant::fromValue( deleteCurveMode ) );

    // grid properties
    gridSizeItem->setData( Qt::DisplayRole, gridSize );

    gridColorItem->setData( Qt::DisplayRole, gridColor );
    gridColorItem->setData( Qt::DecorationRole, gridColor );
    gridColorItem->setData( Qt::UserRole, QVariant::fromValue( gridColor ) );

}
