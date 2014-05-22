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
    _graphicsView( new QArrangementLabellingGraphicsView( this ) ),
    _scene( new QGraphicsScene( -100, -100, 100, 100 ) ),
    _layout( new QGridLayout( this ) ),
    _arrangementGraphicsItem( NULL ),
    _curveInputCallback( NULL ),
    _deleteCurveCallback( NULL ),
    _pointLocationCallback( NULL ),
    _mergeEdgeCallback( NULL ),
    _splitEdgeCallback( NULL ),
    _fillFaceCallback( NULL )
{
    setAttribute( Qt::WA_AlwaysShowToolTips);
    this->setupUi( );
}

void QArrangementLabellingTabBase::setupUi( )
{
    this->_layout->addWidget( this->_graphicsView, 0, 0, 1, 1 );
    this->_graphicsView->setScene( this->_scene );
    //very WIP WIP
    //4this->_graphicsView->move(1000,1000);
    //WIP end
    this->_graphicsView->setMouseTracking( true );
}
//WIP
void QArrangementLabellingTabBase::UpdateFaceLabel(){//DEF : Affiche une infobulle lorsque l'on passe sur une face de l'arrangement, avec son Label
    setStyleSheet("QToolTip { color: white; background-color: #2a82da; border: 1px solid white; }");
    setToolTip(QArrangementLabellingInfoWidget::instance()->FaceLabel());
}

//WIP
QGraphicsScene* QArrangementLabellingTabBase::getScene( ) const
{
    return this->_scene;
}

QArrangementLabellingGraphicsView* QArrangementLabellingTabBase::getView( ) const
{
    return this->_graphicsView;
}

CGAL::Qt::ArrangementGraphicsItemBase*
QArrangementLabellingTabBase::getArrangementGraphicsItem( ) const
{
    return this->_arrangementGraphicsItem;
}

CGAL::Qt::GraphicsViewCurveInputBase*
QArrangementLabellingTabBase::getCurveInputCallback( ) const
{
    return this->_curveInputCallback;
}

CGAL::Qt::Callback* QArrangementLabellingTabBase::getDeleteCurveCallback( ) const
{
    return this->_deleteCurveCallback;
}

CGAL::Qt::Callback* QArrangementLabellingTabBase::getPointLocationCallback( ) const
{
    return this->_pointLocationCallback;
}

CGAL::Qt::Callback* QArrangementLabellingTabBase::getMergeEdgeCallback( ) const
{
    return this->_mergeEdgeCallback;
}

SplitEdgeCallbackBase* QArrangementLabellingTabBase::getSplitEdgeCallback( ) const
{
    return this->_splitEdgeCallback;
}

FillFaceCallbackBase* QArrangementLabellingTabBase::getFillFaceCallback( ) const
{
    return this->_fillFaceCallback;
}
void QArrangementLabellingTabBase::FaceLabelToolTip(){
    setToolTip(QArrangementLabellingInfoWidget::instance()->FaceLabel());
}

bool QArrangementLabellingTabBase::arrHasBeenSaved()
{
    return !_arrangementGraphicsItem->changed;
}

void QArrangementLabellingTabBase::setArrHasBeenSaved( bool saved )
{
    _arrangementGraphicsItem->changed = !saved; QArrangementLabellingInfoWidget::instance()->setChanged(!saved);
}
