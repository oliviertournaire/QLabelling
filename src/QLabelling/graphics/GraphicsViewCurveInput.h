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

#ifndef CGAL_QT_GRAPHICS_VIEW_CURVE_INPUT_H
#define CGAL_QT_GRAPHICS_VIEW_CURVE_INPUT_H

#include <iostream>
#include <CGAL/Arr_polyline_traits_2.h>
#include <CGAL/Qt/GraphicsViewInput.h>
#include <CGAL/Qt/Converter.h>
#include <QEvent>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

#include "Callback.h"
#include "QArrangementLabellingInfoWidget.h"
#include "ISnappable.h"
#include "PointsGraphicsItem.h"



//WIP
//class VanishingPoint : public Singleton<QArrangementLabellingWidget>{
//public :
//   CGAL::Arr_polyline_traits_2::Point_2 VanishingPoint(int nb) ;//trouve le point de fuite numéro nb
//    virtual void SetVanishingPoint(int nb, CGAL::Arr_polyline_traits_2::Point_2 NewVanishingPoint ) ;//associe de nouvelle coordonnées au point de fuite nb
//private:

//    CGAL::Arrangement_with_history_2::Point_2* VanishingP; //liste des points de fuite
//}

// void VanishingPoint::SetVanishingPoint(int nb, CGAL::Arr_polyline_traits_2::Point_2 NewVanishingPoint ){
//    if (nb>VanishingP->dimension()){
//        CGAL::Point_2* VanishingPCopy=VanishingP;
//        VanishingP=new CGAL::Point_2[nb];
//        for(int i=0;i<nb-1;i++)
//            VanishingP[i]=VanishingPCopy[i];
//        VanishingP[nb-1]=NewVanishingPoint;
//        delete [] VanishingPCopy;

//    }
//    else
//        VanishingP[nb]=NewVanishingPoint;
//}
//CGAL::Point_2 VanishingPoint::VanishingPoint(int nb){
//    return VanishingP[nb];

//}

//WIP
enum InsertType {
    POLYLINE, HORIZONTAL, VERTICAL,DEFINE_VANISHING,USE_VANISHING
};
    
namespace CGAL {
namespace Qt {

class GraphicsViewCurveInputBase : public GraphicsViewInput, public ISnappable, public QGraphicsSceneMixin
{
public:
    /**
     Add our helper graphics items to the scene.
     @override
  */
    virtual void setScene( QGraphicsScene* scene );
    virtual QGraphicsScene* getScene( ) const;

    void setSnappingEnabled( bool b );
    void setSnapToGridEnabled( bool b );
    virtual void setColor( QColor c );
    QColor getColor( ) const;
    
    // Type d'insertion à l'intérieur de l'onglet courant
    InsertType _mode;

protected:
    GraphicsViewCurveInputBase( QObject* parent );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* event );
    virtual bool eventFilter( QObject* obj, QEvent* event );

    PointsGraphicsItem _pointsGraphicsItem; // shows user specified curve points
    bool _snappingEnabled;
    bool _snapToGridEnabled;
    QColor _color;

}; // class GraphicsViewCurveInputBase

template < typename ArrTraits >
class GraphicsViewCurveInput : public GraphicsViewCurveInputBase { };

/**
   Specialization of GraphicsViewCurveInput for Arr_polyline_traits_2; handles
   user-guided generation of line segment curves.
*/
template < typename SegmentTraits >
class GraphicsViewCurveInput< CGAL::Arr_polyline_traits_2< SegmentTraits > >:
        public GraphicsViewCurveInputBase
{
public:
    typedef CGAL::Arr_polyline_traits_2< SegmentTraits > Traits;
    typedef typename Traits::Curve_2 Curve_2;
    typedef typename SegmentTraits::Kernel Kernel;
    typedef typename Kernel::Point_2 Point_2;
    typedef typename Kernel::Segment_2 Segment_2;

    GraphicsViewCurveInput( QObject* parent ):
        GraphicsViewCurveInputBase( parent )
    { }

protected:
    void mouseMoveEvent( QGraphicsSceneMouseEvent* event )
    {
        if ( ! this->_polylineGuide.empty( ) )
        {
            Point_2 clickedPoint = this->snapPoint( event );
            // TODO: make it work for the latest line segment
            Segment_2 segment( this->_points.back( ), clickedPoint );
            QLineF qSegment = this->_converter( segment );
            this->_polylineGuide.back( )->setLine( qSegment );
        }
    }

    void mousePressEvent( QGraphicsSceneMouseEvent* event )
    {
        Point_2 clickedPoint = this->snapPoint( event );
	
	    if( _mode == POLYLINE )
        {
	        if ( this->_points.empty( ) )
            {
                // first
                // add clicked point to polyline
                this->_points.push_back( clickedPoint );
                // std::cout << "Insertion d'un point (premier de la ligne) en " << clickedPoint << "." << std::endl;

                QPointF pt = this->_converter( clickedPoint );
                QGraphicsLineItem* lineItem = new QGraphicsLineItem( pt.x( ), pt.y( ), pt.x( ), pt.y( ) );
                lineItem->setZValue( 100 );
                QPen pen = lineItem->pen( );
                pen.setColor( this->_color );
                lineItem->setPen( pen );
                this->_polylineGuide.push_back( lineItem );
                if ( this->_scene != NULL )
                {
                    this->_scene->addItem( this->_polylineGuide.back( ) );
                }
	        }
	        else
	        {
		        // add clicked point to polyline
                if ( event->button( ) == ::Qt::MiddleButton )
                {
                    // finalize polyline input
                    clickedPoint = this->_points.front();
                }
                
                // Fix #37: when the user clicks the same point twice, do not add it to the current polyline and exit now
                if(clickedPoint == _points.back())
                    return;
		        this->_points.push_back( clickedPoint );
                
                // #37 (https://github.com/oliviertournaire/QLabelling/issues/37): This is where the crash happens when the user click consecutively the same point
                Curve_2 res( this->_points.begin( ), this->_points.end( ) );
                emit generate( CGAL::make_object( res ) );
                
                switch(event->button( ))
                {
                    case ::Qt::MiddleButton :
                    case ::Qt::RightButton : // finalize polyline input
                        // Destruction de la Polyline courante
                        for ( unsigned int i = 0; i < this->_polylineGuide.size( ); ++i )
                        {
                            if ( this->_scene != NULL )
                            {
                                this->_scene->removeItem( this->_polylineGuide[ i ] );
                            }
                            delete this->_polylineGuide[ i ];
                        }
                        this->_polylineGuide.clear( );
                        this->_points.clear( );
                        
                        QArrangementLabellingInfoWidget::instance()->setChanged( true );
                        
                        break;
                    default:
                        QPointF pt = this->_converter( clickedPoint );
                        QGraphicsLineItem* lineItem = new QGraphicsLineItem( pt.x( ), pt.y( ), pt.x( ), pt.y( ) );
                        lineItem->setZValue( 100 );
                        QPen pen = lineItem->pen( );
                        pen.setColor( this->_color );
                        lineItem->setPen( pen );
                        this->_polylineGuide.push_back( lineItem ); // Ajout à l'objet Polyline de ce nouveau segment
                        if ( this->_scene != NULL )
                        {
                            this->_scene->addItem( this->_polylineGuide.back( ) ); // Ajout à la scène du dernier segment (celui qui vient d'être ajouté)
                        }
                }
	        }
	    }
	    else if ( _mode == HORIZONTAL )
	    {
            // Ligne horizontale
	        QRect size_imagetolabel(0,0,1000,1000);
	    
	        QGraphicsScene* currentScene = this->_scene;
	        QList<QGraphicsItem*> allItems = currentScene->items();
	        for(int i=0;i<allItems.count();++i)
            {
		        if( QGraphicsPixmapItem *p = qgraphicsitem_cast<QGraphicsPixmapItem*>(allItems[i]) )
		            size_imagetolabel = p->pixmap().rect();
	        }
	    
	        QPointF pt = this->_converter( clickedPoint );
	        Point_2 g(0, (int) pt.y());
	        Point_2 d(size_imagetolabel.width(), (int) pt.y());
	        this->_points.push_back( g );
	        this->_points.push_back( d );
	        Curve_2 res( this->_points.begin( ), this->_points.end( ) );
	        this->_points.clear( );
	        emit generate( CGAL::make_object( res ) );
	    }
	    else if ( _mode == VERTICAL )
	    {
            // Ligne verticale
	        QRect size_imagetolabel(0,0,1000,1000);
	    
	        QGraphicsScene* currentScene = this->_scene;
	        QList<QGraphicsItem*> allItems = currentScene->items();
	        for(int i=0;i<allItems.count();++i)
            {
		        if( QGraphicsPixmapItem *p = qgraphicsitem_cast<QGraphicsPixmapItem*>(allItems[i]) )
		            size_imagetolabel = p->pixmap().rect();
	        }
	    
	        QPointF pt = this->_converter( clickedPoint );
	        Point_2 g((int) pt.x(), 0);
	        Point_2 d((int) pt.x(), size_imagetolabel.height());
	        this->_points.push_back( g );
	        this->_points.push_back( d );
	        Curve_2 res( this->_points.begin( ), this->_points.end( ) );
	        this->_points.clear( );
	        emit generate( CGAL::make_object( res ) );
        }
    //WIP
//    else if ( mode == USE_VANISHING)
//	{ // Tracer à l'aide du point de fuite
//	    QRect size_imagetolabel(0,0,1000,1000);

//	    QGraphicsScene* currentScene = this->_scene;
//	    QList<QGraphicsItem*> allItems = currentScene->items();
//	    for(int i=0;i<allItems.count();++i){
//		if( QGraphicsPixmapItem *p = qgraphicsitem_cast<QGraphicsPixmapItem*>(allItems[i]) )
//		    size_imagetolabel = p->pixmap().rect();
//	    }

//	    QPointF pt = this->convert( clickedPoint );
//        //Ici, il faut un moyen de stocker le point sous forme de singleton et de type compatiple avec les autres points
//        //Ensuite, on calcul l'equation de la droite qui passe entre ce point et le point cliqué
//        //f(x)=a*x+b
//        //on trouve a=(y2-y1)/(x2-x1) et b=(x1y2-x2y1)/(x2-x1)
//        //on réalise ensuite un test pour voir sur quelles bordures se trouve le point : on teste donc si f(0) est compris entre 0 et size_imagetolabel.height() :
//        //si oui, on prend Point_2 g(0,(int)f(0));
//        //si non, on calcul x pour y=0 => Point_2 g((int)-b/a,0)
//        //Prévoir tous les risques avec un autre cas : hors cadre => on trace rien
//        //On applique les mêmes étapes pour le second point
//	    Point_2 g(0, (int) pt.y());
//	    Point_2 d(size_imagetolabel.width(), (int) pt.y());
//	    this->points.push_back( g );
//	    this->points.push_back( d );
//	    Curve_2 res( this->points.begin( ), this->points.end( ) );
//	    this->points.clear( );
//	    emit generate( CGAL::make_object( res ) );
//	}
    //WIP
    }

    // override this to snap to the points you like
    virtual Point_2 snapPoint( QGraphicsSceneMouseEvent* event )
    {
        Point_2 clickedPoint = this->_converter( event->scenePos( ) );
        return clickedPoint;
    }

    Converter< Kernel > _converter;
    std::vector< Point_2 > _points;

    std::vector< QGraphicsLineItem* > _polylineGuide; // Polyline courante
};

} // namespace Qt
} // namespace CGAL

#endif // CGAL_QT_GRAPHICS_VIEW_SEGMENT_INPUT_H
