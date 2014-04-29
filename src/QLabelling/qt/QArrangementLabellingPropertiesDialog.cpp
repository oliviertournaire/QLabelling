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

#include "QArrangementLabellingPropertiesDialog.h"
#include "ui_QArrangementLabellingPropertiesDialog.h"
#include "QArrangementLabellingWindow.h"
#include "PropertyValueDelegate.h"
#include "DeleteCurveMode.h"
#include "QArrangementLabellingGraphicsView.h"

QArrangementLabellingPropertiesDialog::QArrangementLabellingPropertiesDialog( QArrangementLabellingWindow* parent_, Qt::WindowFlags f ) :
    QDialog( parent_, f ),
    parent( parent_ ),
    ui( new Ui::QArrangementLabellingPropertiesDialog )
{
    this->setupUi( );
}

QVariant QArrangementLabellingPropertiesDialog::property( int index )
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
    if ( qVariantCanConvert< QColor >( myData ) || qVariantCanConvert< DeleteCurveMode >( myData ) )
    {
        return myData;
    }
    else
    {
        res = item->data( Qt::DisplayRole );
    }

    return res;
}

void QArrangementLabellingPropertiesDialog::setupUi( )
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
    QTableWidgetItem* gridSnappingDistanceItem = new QTableWidgetItem;
    QTableWidgetItem* vertexSnappingDistanceItem = new QTableWidgetItem;

    this->ui->tableWidget->setItem( int(EDGE_COLOR_KEY), 0, edgeColorItem );
    this->ui->tableWidget->setItem( int(VERTEX_COLOR_KEY), 0, vertexColorItem );
    this->ui->tableWidget->setItem( int(EDGE_WIDTH_KEY), 0, edgeWidthItem );
    this->ui->tableWidget->setItem( int(VERTEX_RADIUS_KEY), 0, vertexRadiusItem );
    this->ui->tableWidget->setItem( int(DELETE_CURVE_MODE_KEY), 0, deleteCurveModeItem );
    this->ui->tableWidget->setItem( int(GRID_SIZE_KEY), 0, gridSizeItem );
    this->ui->tableWidget->setItem( int(GRID_COLOR_KEY), 0, gridColorItem );
    this->ui->tableWidget->setItem( int(GRID_SNAPPING_DISTANCE), 0, gridSnappingDistanceItem );
    this->ui->tableWidget->setItem( int(VERTEX_SNAPPING_DISTANCE), 0, vertexSnappingDistanceItem );

    // fill in the items with data
    this->updateUi( );
}

void QArrangementLabellingPropertiesDialog::updateUi( )
{
    if ( this->parent == NULL )
    {
        return;
    }
    QArrangementLabellingTabBase* currentTab = this->parent->getCurrentTab( );
    if ( currentTab == NULL )
    {
        return;
    }
    CGAL::Qt::ArrangementGraphicsItemBase* agi = currentTab->getArrangementGraphicsItem( );
    if ( agi == NULL )
        return;

    QArrangementLabellingGraphicsView* view = currentTab->getView( );
    QPen vertexPen                      = agi->verticesPen( );
    QPen edgePen                        = agi->edgesPen( );
    QBrush vertexPenBrush               = vertexPen.brush( );
    QBrush edgePenBrush                 = edgePen.brush( );
    QColor vertexColor                  = vertexPenBrush.color( );
    QColor edgeColor                    = edgePenBrush.color( );
    unsigned int edgeWidth              = edgePen.width( );
    unsigned int vertexRadius           = vertexPen.width( );
    unsigned int gridSize               = view->gridSize();
    QColor gridColor                    = view->gridColor();
    unsigned int gridSnappingDistance   = view->gridSnappingDistance();
    unsigned int vertexSnappingDistance = view->vertexSnappingDistance();
    
    QTableWidgetItem* edgeColorItem              = this->ui->tableWidget->item( int(EDGE_COLOR_KEY), 0 );
    QTableWidgetItem* edgeWidthItem              = this->ui->tableWidget->item( int(EDGE_WIDTH_KEY), 0 );
    QTableWidgetItem* vertexColorItem            = this->ui->tableWidget->item( int(VERTEX_COLOR_KEY), 0 );
    QTableWidgetItem* vertexRadiusItem           = this->ui->tableWidget->item( int(VERTEX_RADIUS_KEY), 0 );
    QTableWidgetItem* deleteCurveModeItem        = this->ui->tableWidget->item( int(DELETE_CURVE_MODE_KEY), 0 );
    QTableWidgetItem* gridSizeItem               = this->ui->tableWidget->item( int(GRID_SIZE_KEY), 0 );
    QTableWidgetItem* gridColorItem              = this->ui->tableWidget->item( int(GRID_COLOR_KEY), 0 );
    QTableWidgetItem* gridSnappingDistanceItem   = this->ui->tableWidget->item( int(GRID_SNAPPING_DISTANCE), 0 );
    QTableWidgetItem* vertexSnappingDistanceItem = this->ui->tableWidget->item( int(VERTEX_SNAPPING_DISTANCE), 0 );

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
    deleteCurveModeItem->setData( Qt::DisplayRole, DeleteCurveMode::ToString( deleteCurveMode ) );
    deleteCurveModeItem->setData( Qt::UserRole, QVariant::fromValue( deleteCurveMode ) );

    // grid properties
    gridSizeItem->setData ( Qt::DisplayRole, gridSize );
    gridColorItem->setData( Qt::DisplayRole, gridColor );
    gridColorItem->setData( Qt::DecorationRole, gridColor );
    gridColorItem->setData( Qt::UserRole, QVariant::fromValue( gridColor ) );

    // Snapping distance
    gridSnappingDistanceItem->setData( Qt::DisplayRole, gridSnappingDistance );
    gridSnappingDistanceItem->setData( Qt::DecorationRole, gridSnappingDistance );
    gridSnappingDistanceItem->setData( Qt::UserRole, gridSnappingDistance );
    vertexSnappingDistanceItem->setData( Qt::DisplayRole, vertexSnappingDistance );
    vertexSnappingDistanceItem->setData( Qt::DecorationRole, vertexSnappingDistance );
    vertexSnappingDistanceItem->setData( Qt::UserRole, vertexSnappingDistance );
}
