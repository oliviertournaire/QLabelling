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

#ifndef ARRANGEMENT_CURVE_INPUT_CALLBACK_H
#define ARRANGEMENT_CURVE_INPUT_CALLBACK_H

#include <CGAL/Qt/GraphicsViewInput.h>
#include <CGAL/Qt/Converter.h>

#include <QEvent>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

#include <iostream>

#include "GraphicsViewCurveInput.h"
#include "Utils.h"
#include "ArrangementObserver.h"

#include "QArrangementLabellingLogWidget.h"

template <typename Arr_, typename ArrTraits = typename Arr_::Geometry_traits_2>
class ArrangementCurveInputCallback:
    public CGAL::Qt::GraphicsViewCurveInput< typename Arr_::Geometry_traits_2 >
{
public:
    typedef Arr_ Arrangement;
    typedef ArrTraits Traits;
    typedef CGAL::Qt::GraphicsViewCurveInput< Traits >    Superclass;
    typedef typename Arrangement::Vertex_iterator         Vertex_iterator;
    typedef typename Arrangement::Face_iterator           Face_iterator;
    typedef typename Traits::Curve_2                      Curve_2;
    typedef typename Traits::X_monotone_curve_2           X_monotone_curve_2;
    typedef typename ArrTraitsAdaptor< Traits >::Kernel   Kernel;
    typedef typename Kernel::Point_2                      Kernel_point_2;
    typedef typename ArrTraitsAdaptor< Traits >::Point_2  Point_2;
    typedef typename Kernel::Segment_2                    Segment_2;
    typedef typename Kernel::FT                           FT;

    ArrangementCurveInputCallback( Arrangement* arrangement, QObject* parent ):
    Superclass( parent ),
    _arrangement( arrangement )
    {
        this->_snapToVertexStrategy.setArrangement( arrangement );

        QObject::connect( this, SIGNAL( generate    ( CGAL::Object ) ),
                          this, SLOT  ( processInput( CGAL::Object ) ) );
    }

    void processInput( CGAL::Object inputObject )
    {
        Curve_2 curve;
        QArrangementLabellingLogWidget::instance()->logDebug( tr("Trying to assign 'CGAL::Object' to 'Curve_2' ...") );
        if ( CGAL::assign( curve, inputObject ) )
        {
            QArrangementLabellingLogWidget::instance()->logTrace(QObject::tr("Inserting object (%n vertices) into the arrangement.","",curve.points()));

            Arrangement_Observer<Arrangement> obs(*( this->_arrangement ));

            QArrangementLabellingLogWidget::instance()->logDebug( tr("Trying to insert curve into the arrangement ...") );
            CGAL::insert( *( this->_arrangement ), curve );
            QArrangementLabellingLogWidget::instance()->logDebug( tr("Curve inserted with success in the arrangement ...") );

            QString message(QObject::tr("Vertices of the arrangement :\n"));
            Vertex_iterator v;
            int index;
            for (v = this->_arrangement->vertices_begin(), index=0 ; v != this->_arrangement->vertices_end(); ++v, ++index)
            {
                message = message + QObject::tr("\t* Found vertex #%n : (","",index);
                message += QString::number(CGAL::to_double(v->point().x()));
                message += ";";
                message += QString::number(CGAL::to_double(v->point().y()));
                message = message + QObject::tr("), degree %n.\n","",(int)v->degree());
            }
            QArrangementLabellingLogWidget::instance()->logTrace(message);
        }
        else
        {
            QArrangementLabellingLogWidget::instance()->logError(QObject::tr("Unable to add the element in the arrangement !"));
	    }

        emit CGAL::Qt::GraphicsViewInput::modelChanged( );
    }

    void setScene( QGraphicsScene* scene )
    {
        this->Superclass::setScene( scene );
        this->_snapToVertexStrategy.setScene( scene );
        this->_snapToGridStrategy.setScene( scene );
    }

    void setArrangement( Arrangement* arrangement )
    {
        this->_arrangement = arrangement;
    }

protected:
    Point_2 snapPoint( QGraphicsSceneMouseEvent* event )
    {
        if ( this->_snapToGridEnabled )
        {
            return this->_snapToGridStrategy.snapPoint( event );
        }
        else if ( this->_snappingEnabled )
        {
            return this->_snapToVertexStrategy.snapPoint( event );
        }
        else
        {
            Kernel_point_2 p = this->_converter( event->scenePos( ) );
            Point_2 res = this->_toArrPoint( p );
            return res;
        }
    }

    Arrangement* _arrangement;
    SnapToArrangementVertexStrategy< Arrangement > _snapToVertexStrategy;
    SnapToGridStrategy< typename Arrangement::Geometry_traits_2 > _snapToGridStrategy;
    Arr_construct_point_2< Traits > _toArrPoint;
}; // class ArrangementCurveInputCallback

#endif // ARRANGEMENT_SEGMENT_INPUT_CALLBACK_H
