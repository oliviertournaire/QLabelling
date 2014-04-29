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

#include <QVarLengthArray>
#include <QPen>

#include "QArrangementLabellingGraphicsView.h"
#include "QArrangementLabellingTab.h"
#include "ArrangementTypes.h"

#include "QArrangementLabellingLogWidget.h"
#include "config.hpp"

QArrangementLabellingGraphicsView::QArrangementLabellingGraphicsView( QWidget* parent ) :
    QGraphicsView( parent ),
    _showGrid( false ),
    _gridSize( 50 ),
    _gridColor( ::Qt::black ),
    _backgroundColor( ::Qt::white ),
    _snappingDistance(5)
{
    QMatrix m( 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 );
    this->setMatrix( m );
    this->setBackgroundBrush( QBrush( _backgroundColor ) );
}

void QArrangementLabellingGraphicsView::drawForeground( QPainter* painter, const QRectF& rect )
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

QRectF QArrangementLabellingGraphicsView::getViewportRect( ) const
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

bool QArrangementLabellingGraphicsView::setImageToLabel(const QString& path, QArrangementLabellingTabBase *currentTab, CGAL::Object currentArrangement)
{
    QPixmap imagetolabel = QPixmap(path);
    setImageToLabel(imagetolabel);
    setImageToLabelFilename(path);
    setImageToLabelSize(imagetolabel.size());

    if(imageToLabel().isNull())
    {
        QArrangementLabellingLogWidget::instance()->logError( tr("Unable to open image ") + path );
        return false;
    }

    // Try to find if an image has already been loaded
    QList<QGraphicsItem*> allItems = currentTab->getScene()->items();
    for(int i=0;i<allItems.count();++i)
        if( QGraphicsPixmapItem *p = qgraphicsitem_cast<QGraphicsPixmapItem*>(allItems[i]) )
            currentTab->getScene()->removeItem(p);
    QGraphicsItem* pixItem = currentTab->getScene()->addPixmap(imageToLabel());
    fitInView(pixItem, Qt::KeepAspectRatio);

    Arr_pol_point_2 ptl( 0, 0);
    Arr_pol_point_2 pbl(0, imageToLabelHeight() );
    Arr_pol_point_2 pbr(imageToLabelWidth(), imageToLabelHeight());
    Arr_pol_point_2 ptr(imageToLabelWidth(), 0 );

    QString imageBoundaryMessage = tr("Image boundaries: ");
    imageBoundaryMessage = imageBoundaryMessage + "(" + QString::number(CGAL::to_double(ptl.x())) + "," + QString::number(CGAL::to_double(ptl.y())) + ") / ";
    imageBoundaryMessage = imageBoundaryMessage + "(" + QString::number(CGAL::to_double(pbl.x())) + "," + QString::number(CGAL::to_double(pbl.y())) + ") / ";
    imageBoundaryMessage = imageBoundaryMessage + "(" + QString::number(CGAL::to_double(pbr.x())) + "," + QString::number(CGAL::to_double(pbr.y())) + ") / ";
    imageBoundaryMessage = imageBoundaryMessage + "(" + QString::number(CGAL::to_double(ptr.x())) + "," + QString::number(CGAL::to_double(ptr.y())) + ")";
    QArrangementLabellingLogWidget::instance()->logTrace( imageBoundaryMessage );

    std::vector<Arr_pol_point_2> allPoints;
    allPoints.push_back(ptl);
    allPoints.push_back(pbl);
    allPoints.push_back(pbr);
    allPoints.push_back(ptr);
    allPoints.push_back(ptl);

    Arr_pol_2 contour(allPoints.begin(), allPoints.end());

    Pol_arr *arr;
    if ( CGAL::assign( arr, currentArrangement ) )
    {
        CGAL::Qt::GraphicsViewCurveInputBase *gvcib = currentTab->getCurveInputCallback();
        ArrangementCurveInputCallback<Pol_arr> *acic = dynamic_cast< ArrangementCurveInputCallback<Pol_arr>* >(gvcib);
        if(acic)
        {
            acic->processInput( CGAL::make_object(contour) );
        }
        // Setting the right label to the newly created frame
        for(Pol_arr::Face_iterator fit = arr->faces_begin() ; fit != arr->faces_end() ; fit++)
        {
            if(!fit->is_unbounded())
            {
                fit->data().set_label(QLABELLING_UNKNOW_LABEL_STRING);
            }
        }
    }
    else
    {
        QArrangementLabellingLogWidget::instance()->logWarning( tr("Unable to retrieve the arrangement!") );
        return false;
    }

    return true;
}