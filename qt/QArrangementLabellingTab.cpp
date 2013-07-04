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

#include "QArrangementLabellingTab.h"

#include <QGridLayout>

QArrangementLabellingTabBase::QArrangementLabellingTabBase( QWidget* parent ) :
    QWidget( parent ),
    graphicsView( new ArrangementDemoGraphicsView( this ) ),
    scene( new QGraphicsScene( -100, -100, 100, 100 ) ),
    layout( new QGridLayout( this ) ),
    arrangementGraphicsItem( NULL ),
    curveInputCallback( NULL ),
    deleteCurveCallback( NULL ),
    pointLocationCallback( NULL ),
    mergeEdgeCallback( NULL ),
    splitEdgeCallback( NULL ),
    fillFaceCallback( NULL )
{
    this->setupUi( );
}

void QArrangementLabellingTabBase::setupUi( )
{
    this->layout->addWidget( this->graphicsView, 0, 0, 1, 1 );
    this->graphicsView->setScene( this->scene );
    this->graphicsView->setMouseTracking( true );
}

QGraphicsScene* QArrangementLabellingTabBase::getScene( ) const
{
    return this->scene;
}

ArrangementDemoGraphicsView* QArrangementLabellingTabBase::getView( ) const
{
    return this->graphicsView;
}

CGAL::Qt::ArrangementGraphicsItemBase*
QArrangementLabellingTabBase::getArrangementGraphicsItem( ) const
{
    return this->arrangementGraphicsItem;
}

CGAL::Qt::GraphicsViewCurveInputBase*
QArrangementLabellingTabBase::getCurveInputCallback( ) const
{
    return this->curveInputCallback;
}

CGAL::Qt::Callback* QArrangementLabellingTabBase::getDeleteCurveCallback( ) const
{
    return this->deleteCurveCallback;
}

CGAL::Qt::Callback* QArrangementLabellingTabBase::getPointLocationCallback( ) const
{
    return this->pointLocationCallback;
}

CGAL::Qt::Callback* QArrangementLabellingTabBase::getMergeEdgeCallback( ) const
{
    return this->mergeEdgeCallback;
}

SplitEdgeCallbackBase* QArrangementLabellingTabBase::getSplitEdgeCallback( ) const
{
    return this->splitEdgeCallback;
}

FillFaceCallbackBase* QArrangementLabellingTabBase::getFillFaceCallback( ) const
{
    return this->fillFaceCallback;
}
