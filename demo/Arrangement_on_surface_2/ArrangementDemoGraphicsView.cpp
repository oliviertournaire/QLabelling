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
    showGrid( false ),
    gridSize( 50 ),
    gridColor( ::Qt::black ),
    backgroundColor( ::Qt::white )
{
    QMatrix m( 1.0, 0.0, 0.0, -1.0, 0.0, 0.0 );
    this->setMatrix( m );
    this->setBackgroundBrush( QBrush( backgroundColor ) );
}

void ArrangementDemoGraphicsView::setShowGrid( bool b )
{
    this->showGrid = b;
}

bool ArrangementDemoGraphicsView::getShowGrid( ) const
{
    return this->showGrid;
}

void ArrangementDemoGraphicsView::setGridSize( int size )
{
    this->gridSize = size;
}

int ArrangementDemoGraphicsView::getGridSize( ) const
{
    return this->gridSize;
}

void ArrangementDemoGraphicsView::setGridColor( QColor color )
{
    this->gridColor = color;
}

QColor ArrangementDemoGraphicsView::getGridColor( ) const
{
    return this->gridColor;
}

void ArrangementDemoGraphicsView::setBackgroundColor( QColor color )
{
    this->backgroundColor = color;
}

QColor ArrangementDemoGraphicsView::getBackgroundColor( ) const
{
    return this->backgroundColor;
}

void ArrangementDemoGraphicsView::drawForeground( QPainter* painter,
                                                  const QRectF& rect )
{
    QRectF viewportRect = this->getViewportRect( );
    if ( this->showGrid )
    {
        // compute integer-spaced grid lines
        QVarLengthArray< QLineF, 100 > linesX;
        QVarLengthArray< QLineF, 100 > linesY;
        qreal left =
                int(viewportRect.left()) - (int(viewportRect.left()) % this->gridSize);
        qreal top =
                int(viewportRect.top()) - (int(viewportRect.top()) % this->gridSize);
        for ( qreal x = left; x < viewportRect.right( ); x += this->gridSize )
        {
            linesX.append( QLineF(x, viewportRect.top(), x, viewportRect.bottom()));
        }
        for ( qreal y = top; y < viewportRect.bottom( ); y += this->gridSize )
        {
            linesY.append(QLineF(viewportRect.left( ), y, viewportRect.right(), y));
        }

        // set up the painter
        QPen savePen = painter->pen( );
        QPen gridPen( savePen );
        gridPen.setColor( this->gridColor );
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

void ArrangementDemoGraphicsView::setImagetoLabel(const QString& filename)
{
    _imageToLabelFilename = filename;
    _imageToLabel = QPixmap(filename);
    if(_imageToLabel.isNull())
        std::cout << "Problem opening image ..." << std::endl;
//     removeAllItemsFromScene();
    this->scene()->addPixmap(_imageToLabel);
    _labelsImage = QImage(_imageToLabel.width(), _imageToLabel.height(), QImage::Format_RGB32);
//    TODO  _labelsImage.fill( QLABELLING_UNKNOW_LABEL_BRUSH_COLOR );
    _labelsImage.fill( QColor(10,10,10) );
//    TODO  buildImageBoundaryRect();
}


void ArrangementDemoGraphicsView::buildImageBoundaryRect()
{
    // TODO
//     QGraphicsRectWithLabelItem* boundaries = new QGraphicsRectWithLabelItem;
//     boundaries->setRect(0, 0, _imageToLabel.width(), _imageToLabel.height());
//     boundaries->setLabel(QLABELLING_UNKNOW_LABEL_STRING);
//     boundaries->setBrush( QLABELLING_UNKNOW_LABEL_BRUSH_COLOR );
//     _scene->addItem(boundaries);
//     _rects.push_back( boundaries );
}

void ArrangementDemoGraphicsView::removeAllItemsFromScene()
{
    // TODO
//     QList<QGraphicsItem *> items = _scene->items();
//     for(int i=0;i<items.size();++i)
//         _scene->removeItem(items[i]);
}

void ArrangementDemoGraphicsView::removeImagesFromScene()
{
    // TODO
//     QList<QGraphicsItem *> items = _scene->items();
//     for(int i=0;i<items.size();++i)
//         if ( qgraphicsitem_cast<QGraphicsPixmapItem*>(items[i]) )
//             _scene->removeItem(items[i]);
}

void ArrangementDemoGraphicsView::removeAllRectanglesFromScene()
{
    // TODO
//     QList<QGraphicsItem *> items = _scene->items();
//     for(int i=0;i<items.size();++i)
//         if ( qgraphicsitem_cast<QGraphicsRectItem*>(items[i]) )
//             _scene->removeItem(items[i]);
}

void ArrangementDemoGraphicsView::removeAllRectanglesWithLabelFromScene()
{
    // TODO
//     for(unsigned int i=0;i<_rects.size();++i)
//         _scene->removeItem(_rects[i]);
}

void ArrangementDemoGraphicsView::rebuildRectanglesFromLastLine(const QLineF& line)
{
    // TODO
//     vector<QGraphicsRectWithLabelItem*> collidingRects;
// 
//     unsigned int numIntersectedRectWithLabel = 0;
//     QGraphicsLineItem lineItem;
//     lineItem.setLine(line);
//     QList<QGraphicsItem *> collidingItems = _scene->collidingItems (&lineItem);
//     for(int i=0;i<collidingItems.size();++i)
//         if ( QGraphicsRectWithLabelItem* collidedRect = qgraphicsitem_cast<QGraphicsRectWithLabelItem*>(collidingItems[i]) )
//         {
//             ++numIntersectedRectWithLabel;
//             collidingRects.push_back(collidedRect);
//         }
// 
//     // Split colliding rects wrt lastLine
//     // 1) Find the rect in the _rects vector
//     for(unsigned int i=0;i<collidingRects.size();++i)
//     {
//         for(unsigned int j=0;j<_rects.size();++j)
//         {
//             if ( collidingRects[i]->rect() == _rects[j]->rect() )
//             {
//                 QString intersectedLabel = _rects[j]->label();
//                 QGraphicsRectWithLabelItem* newRect1 = new QGraphicsRectWithLabelItem;
//                 QGraphicsRectWithLabelItem* newRect2 = new QGraphicsRectWithLabelItem;
// 
//                 // Vertical Line
//                 if ( line.x1() == line.x2() )
//                 {
//                     newRect1->setRect( collidingRects[i]->rect().x(),
//                                        collidingRects[i]->rect().y(),
//                                        line.x1()-collidingRects[i]->rect().x(),
//                                        collidingRects[i]->rect().height() );
// 
//                     newRect2->setRect( line.x1(),
//                                        collidingRects[i]->rect().y(),
//                                        collidingRects[i]->rect().right()-line.x1(),
//                                        collidingRects[i]->rect().height() );
//                 }
//                 // Horizontal line
//                 else
//                 {
//                     newRect1->setRect( collidingRects[i]->rect().x(),
//                                        collidingRects[i]->rect().y(),
//                                        collidingRects[i]->rect().width(),
//                                        line.y1()-collidingRects[i]->rect().y() );
// 
//                     newRect2->setRect( collidingRects[i]->rect().x(),
//                                        line.y1(),
//                                        collidingRects[i]->rect().width(),
//                                        collidingRects[i]->rect().bottom()-line.y1() );
//                 }
//                 vector<QGraphicsRectWithLabelItem*>::iterator it = std::find(_rects.begin(), _rects.end(), _rects[j]);
//                 _scene->removeItem(collidingRects[i]);
//                 _rects.erase(it);
//                 newRect1->setLabel(intersectedLabel);
//                 newRect2->setLabel(intersectedLabel);
//                 _rects.push_back(newRect1);
//                 _rects.push_back(newRect2);
// 
//                 QBrush brush(Qt::SolidPattern);
// 
//                 const QLabelItem *intersectedLabelItem = _labellingWidget->findLabelItemFromName(intersectedLabel);
//                 if ( intersectedLabelItem )
//                     brush.setColor( intersectedLabelItem->labelColor() );
//                 else
//                     qDebug() << __FUNCTION__ << "No active label found!!!";
// 
//                 newRect1->setBrush(brush);
//                 newRect2->setBrush(brush);
// 
//                 _scene->addItem(newRect1);
//                 _scene->addItem(newRect2);
//                 _scene->update();
//             }
//         }
//     }
}
