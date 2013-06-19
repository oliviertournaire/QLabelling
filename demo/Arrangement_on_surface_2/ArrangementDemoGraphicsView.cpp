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

#include "ArrangementDemoGraphicsView.h"

#include <iostream>
#include <QVarLengthArray>
#include <QPen>

ArrangementDemoGraphicsView::ArrangementDemoGraphicsView( QWidget* parent ) :
    QGraphicsView( parent ),
    _showGrid( false ),
    _gridSize( 50 ),
    _gridColor( ::Qt::black ),
    _backgroundColor( ::Qt::white )
{
    QMatrix m( 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 );
    this->setMatrix( m );
    this->setBackgroundBrush( QBrush( _backgroundColor ) );
//     this->setAlignment(Qt::AlignCenter); Useless
}

void ArrangementDemoGraphicsView::drawForeground( QPainter* painter, const QRectF& rect )
{
    QRectF viewportRect = this->getViewportRect();
    if ( this->_showGrid )
    {
        // compute integer-spaced grid lines
        QVarLengthArray< QLineF, 100 > linesX;
        QVarLengthArray< QLineF, 100 > linesY;
        qreal left = int(viewportRect.left()) - (int(viewportRect.left()) % this->_gridSize);
        qreal top = int(viewportRect.top()) - (int(viewportRect.top()) % this->_gridSize);
        for ( qreal x = left; x < viewportRect.right( ); x += this->_gridSize )
        {
            linesX.append( QLineF(x, viewportRect.top(), x, viewportRect.bottom()));
        }
        for ( qreal y = top; y < viewportRect.bottom( ); y += this->_gridSize )
        {
            linesY.append(QLineF(viewportRect.left( ), y, viewportRect.right(), y));
        }

        // set up the painter
        QPen savePen = painter->pen( );
        QPen gridPen( savePen );
        gridPen.setColor( this->_gridColor );
        painter->setPen( gridPen );

        // draw the grid
        painter->drawLines( linesX.data( ), linesX.size( ) );
        painter->drawLines( linesY.data( ), linesY.size( ) );

        // revert the painter
        painter->setPen( savePen );
    }
}

QRectF ArrangementDemoGraphicsView::getViewportRect( ) const
{
    QPointF p1 = this->mapToScene( 0, 0 );
    QPointF p2 = this->mapToScene( this->width( ), this->height( ) );

    double xmin = std::min( p1.x( ), p2.x( ) );
    double xmax = std::max( p1.x( ), p2.x( ) );
    double ymin = std::min( p1.y( ), p2.y( ) );
    double ymax = std::max( p1.y( ), p2.y( ) );

    QRectF res = QRectF( QPointF( xmin, ymin ), QPointF( xmax, ymax ) );

    return res;
}

void ArrangementDemoGraphicsView::setImageToLabel(const QString& path)
{
    QPixmap imagetolabel = QPixmap(path);
    setImageToLabel(imagetolabel);
    setImageToLabelFilename(path);
    setImageToLabelSize(imagetolabel.size());
}