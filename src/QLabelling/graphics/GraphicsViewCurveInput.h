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

#include "graphics/VanishingPoints.h"







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

        if(VanishingPoints::instance()->countervanishing==1){
            Point_2 clickedPoint = this->snapPoint( event );
            Point_2  pp=VanishingPoints::instance()->GetPointForVanishing();
            Segment_2 segment( pp, clickedPoint );
            QLineF qSegment = this->_converter( segment );
            int index=VanishingPoints::instance()->getIndexSelected();
            if(VanishingPoints::instance()->_VanishingLineGuide.size()==0){
                  QPointF pt = this->_converter( clickedPoint );
                  QPointF px= this->_converter( pp );
                QGraphicsLineItem* qSegprov=new QGraphicsLineItem(px.x(),px.y(),pt.x(),pt.y());

                VanishingPoints::instance()->_VanishingLineGuide.push_back(qSegprov);
            }
            else
                VanishingPoints::instance()->_VanishingLineGuide.back( )->setLine( qSegment );
        }
    }
    void mousePressEvent( QGraphicsSceneMouseEvent* event )
    {
        Point_2 clickedPoint = this->snapPoint( event );

//        // Testing
//        VanishingPoints::instance()->addVanishingPoint(clickedPoint);
//        Point_2 anotherVP(452., 4566.65);
//        VanishingPoints::instance()->addVanishingPoint(anotherVP);
//        Point_2 vp, vp2;
//        try
//        {
//            vp = VanishingPoints::instance()->getVanishingPoints(0);
//            vp2 = VanishingPoints::instance()->getVanishingPoints(10);
//        }
//        catch(const std::out_of_range& e)
//        {
//            std::cout << e.what() << std::endl;
//        }

//        unsigned int nbVP = VanishingPoints::instance()->size();
//        for(unsigned int i=0;i<nbVP;++i)
//        {
//            vp = VanishingPoints::instance()->getVanishingPoints(i);
//            std::cout << vp << std::endl;
//        }

//        vp.x();
//        vp.y();

	
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
        else if ( _mode == VERTICAL)
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

  else if (_mode == USE_VANISHING)
    { // Tracer à l'aide du point de fuite
        QRect size_imagetolabel(0,0,1000,1000);

        QGraphicsScene* currentScene = this->_scene;
        QList<QGraphicsItem*> allItems = currentScene->items();
        for(int i=0;i<allItems.count();++i){
        if( QGraphicsPixmapItem *p = qgraphicsitem_cast<QGraphicsPixmapItem*>(allItems[i]) )
            size_imagetolabel = p->pixmap().rect();
        }

        QPointF pt = this->_converter( clickedPoint );
        Point_2 vp0=VanishingPoints::instance()->getVanishingPoints(0);
         QPointF vp = this->_converter( vp0 );
        //Ici, il faut un moyen de stocker le point sous forme de singleton et de type compatiple avec les autres points
        //Ensuite, on calcul l'equation de la droite qui passe entre ce point et le point cliqué

        qreal a=(vp.y()-pt.y())/(vp.x()-pt.x());
        qreal b=(vp.x()*pt.y()-vp.y()*pt.x())/(vp.x()-pt.x());
        //ATTENTION : ne prend pas en compte la division par 0
        //on trouve a=(y2-y1)/(x2-x1) et b=(x1y2-x2y1)/(x2-x1)
        //on réalise ensuite un test pour voir sur quelles bordures se trouve le point : on teste donc si f(0) est compris entre 0 et size_imagetolabel.height() :
        //si oui, on prend Point_2 g(0,(int)f(0));
        //si non, on calcul x pour y=0 => Point_2 g((int)-b/a,0)
        //Prévoir tous les risques avec un autre cas : hors cadre => on trace rien
        //On applique les mêmes étapes pour le second point



        qreal limit1=VanishingPoints::instance()->Affine(0,a,b);
        qreal limit2=VanishingPoints::instance()->Affine(size_imagetolabel.width(),a,b);
        int imwidth=size_imagetolabel.width();
        int imheight=size_imagetolabel.height();

        int counter=0;//Permet de savoir combien de points ont étés fixés sur la bordure. 1=>il reste d à fixer; 2=>plus de points à fixer
        Point_2 g;
        Point_2 d;
        //On place le point à gauche
        if (limit1>=0 && limit1<=imheight){
            Point_2 g1(0, (int) limit1);
            g=g1;
            counter+=1;}
        if((-b/a)>0 && (-b/a)<imwidth){
            if(counter==0){
                Point_2 g1((int)-b/a, 0);
                g=g1;
                counter+=1;
            }
            else if(counter==1){
                Point_2 d1((int)-b/a, 0);
                d=d1;
                counter+=1;
            }
        }

       //on place le point à droite
        double c=(imheight-b)/a;
        if (limit2>=0 && limit2<=imheight){
            if(counter==0){
                Point_2 g1(imwidth, (int) limit2);
                g=g1;
                counter+=1;
            }
            else if(counter==1){
                Point_2 d1(imwidth, (int) limit2);
                d=d1;
                counter+=1;
            }
        }
        if(c>0 && c<imwidth && counter==1){
            Point_2 d1((int)c, imheight);
            d=d1;
            counter+=1;
        }
        if(counter==2){//On vérifie que l'on passe bien par le dessin car counter<2 => Hors du dessin
            this->_points.push_back( g );
            this->_points.push_back( d );
            Curve_2 res( this->_points.begin( ), this->_points.end( ) );
            this->_points.clear( );
            emit generate( CGAL::make_object( res ) );
        }
    }
        else if( _mode == DEFINE_VANISHING )
        {   int index=VanishingPoints::instance()->getIndexSelected();
            if ( VanishingPoints::instance()->countervanishing==0)
            {
                // first
                // add clicked point to polyline
                VanishingPoints::instance()->SetPointForVanishing( clickedPoint );
                // std::cout << "Insertion d'un point (premier de la ligne) en " << clickedPoint << "." << std::endl;

                QPointF pt = this->_converter( clickedPoint );
                QGraphicsLineItem* lineItem = new QGraphicsLineItem( pt.x( ), pt.y( ), pt.x( ), pt.y( ) );
                lineItem->setZValue( 100 );
                QPen pen = lineItem->pen( );
                pen.setColor( this->_color );
                lineItem->setPen( pen );
                VanishingPoints::instance()->_VanishingLineGuide.push_back( lineItem );
                if ( this->_scene != NULL )
                {
                    this->_scene->addItem( VanishingPoints::instance()->_VanishingLineGuide.back( ) );
                }
                VanishingPoints::instance()->countervanishing=1;
           }
            else
            {


                // Fix #37: when the user clicks the same point twice, do not add it to the current polyline and exit now
                if(clickedPoint == VanishingPoints::instance()->GetPointForVanishing())
                    return;
                 QPointF point1 = this->_converter( clickedPoint );
                QPointF point2=this->_converter(VanishingPoints::instance()->GetPointForVanishing());
                qreal x1=point2.x();
                VanishingPoints::instance()->addVanishingEdges(point1.x(),point1.y(),point2.x(),point2.y(),0);
                this->_points.push_back( clickedPoint );

                // #37 (https://github.com/oliviertournaire/QLabelling/issues/37): This is where the crash happens when the user click consecutively the same point
               std::vector<Point_2>  pp;
               pp.push_back(VanishingPoints::instance()->GetPointForVanishing());

                VanishingPoints::instance()->countervanishing=0;

                switch(event->button( ))
                {
                    case ::Qt::MiddleButton :
                    case ::Qt::RightButton : // finalize polyline input
                        // Destruction de la Polyline courante
                        for ( unsigned int i = 0; i < VanishingPoints::instance()->_VanishingLineGuide.size( ); ++i )
                        {
                            if ( this->_scene != NULL )
                            {
                                this->_scene->removeItem( VanishingPoints::instance()->_VanishingLineGuide[ i ] );
                            }
                            delete VanishingPoints::instance()->_VanishingLineGuide[ i ];
                        }
                        VanishingPoints::instance()->_VanishingLineGuide.clear( );
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
                        VanishingPoints::instance()->_VanishingLineGuide.push_back( lineItem ); // Ajout à l'objet Polyline de ce nouveau segment
                        if ( this->_scene != NULL )
                        {
                            this->_scene->addItem( VanishingPoints::instance()->_VanishingLineGuide.back( ) ); // Ajout à la scène du dernier segment (celui qui vient d'être ajouté)
                        }
                }
            }
        }
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
