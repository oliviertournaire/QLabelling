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
#include "QArrangementLabellingVanishingPointsWidget.h"

#include "graphics/VanishingPoints.h"
#include "findnearestedge.h"
#include "CopyFaceSingleton.h"







enum InsertType {
    POLYLINE, HORIZONTAL, VERTICAL,DEFINE_VANISHING,USE_VANISHING,COPY_FACE
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
    {if(_mode!= DEFINE_VANISHING){
            for ( unsigned int i = 0; i < VanishingPoints::instance()->_VanishingLineGuide.size( ); ++i )
            {
                if ( this->_scene != NULL )
                {
                    this->_scene->removeItem( VanishingPoints::instance()->_VanishingLineGuide[ i ] );
                }
                delete VanishingPoints::instance()->_VanishingLineGuide[ i ];
            }
            VanishingPoints::instance()->_VanishingLineGuide.clear( );
        }
        if( _mode == POLYLINE ){
        if ( ! this->_polylineGuide.empty( ) )
        {
            Point_2 clickedPoint = this->snapPoint( event );
            // TODO: make it work for the latest line segment
            Segment_2 segment( this->_points.back( ), clickedPoint );
            QLineF qSegment = this->_converter( segment );
            this->_polylineGuide.back( )->setLine( qSegment );
        }
        }
        if( _mode == HORIZONTAL ){

                    Point_2 clickedPoint = this->snapPoint( event );
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
                    // TODO: make it work for the latest line segment
                    Segment_2 segment( g, d );
                    QLineF qSegment = this->_converter( segment );

                    if (this->_horizontalGuide.size()==0 ){
                        QPointF pg=this->_converter(g);
                        QPointF pd=this->_converter(d);
                        QGraphicsLineItem* lineItem = new QGraphicsLineItem( pg.x( ), pg.y( ), pd.x( ), pd.y( ) );
                        lineItem->setZValue( 100 );
                        QPen pen = lineItem->pen( );
                        pen.setColor( QColor(255, 175, 0));
                        lineItem->setPen( pen );
                        this->_horizontalGuide.push_back( lineItem );
                        if ( this->_scene != NULL )
                        {
                            this->_scene->addItem( this->_horizontalGuide.back( ) );
                        }
                    }
                    else
                    {
                         Point_2 sizepoint(size_imagetolabel.width(), size_imagetolabel.height());
                         QPointF psizepoint=this->_converter(sizepoint);
                         int hsize=(int) psizepoint.y();
                         int hsnap=(int)pt.y();
                         int vsize=(int) psizepoint.x();
                         int vsnap=(int)pt.x();
                        if(hsnap<0 || hsnap>hsize || vsnap<0 || vsnap>vsize){
                            if ( this->_scene != NULL )
                            {
                                this->_scene->removeItem( this->_horizontalGuide.back());
                            }
                            return;
                        }

                        if ( this->_scene != NULL )
                        {
                            this->_scene->removeItem( this->_horizontalGuide.back());
                        }
                       this->_horizontalGuide.back( )->setLine( qSegment );
                        if ( this->_scene != NULL )
                        {
                            this->_scene->addItem( this->_horizontalGuide.back( ) );
                        }

                    }

                }
        if( _mode == VERTICAL ){

                    Point_2 clickedPoint = this->snapPoint( event );
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
                    // TODO: make it work for the latest line segment
                    Segment_2 segment( g, d );
                    QLineF qSegment = this->_converter( segment );

                    if (this->_horizontalGuide.size()==0 ){
                        QPointF pg=this->_converter(g);
                        QPointF pd=this->_converter(d);
                        QGraphicsLineItem* lineItem = new QGraphicsLineItem( pg.x( ), pg.y( ), pd.x( ), pd.y( ) );
                        lineItem->setZValue( 100 );
                        QPen pen = lineItem->pen( );
                        pen.setColor( QColor(255, 175, 0));
                        lineItem->setPen( pen );
                        this->_horizontalGuide.push_back( lineItem );
                        if ( this->_scene != NULL )
                        {
                            this->_scene->addItem( this->_horizontalGuide.back( ) );
                        }
                    }
                    else
                    {
                        Point_2 sizepoint(size_imagetolabel.width(), size_imagetolabel.height());
                        QPointF psizepoint=this->_converter(sizepoint);
                        int hsize=(int) psizepoint.y();
                        int hsnap=(int)pt.y();
                        int vsize=(int) psizepoint.x();
                        int vsnap=(int)pt.x();
                       if(hsnap<0 || hsnap>hsize || vsnap<0 || vsnap>vsize){
                            if ( this->_scene != NULL )
                            {
                                this->_scene->removeItem( this->_horizontalGuide.back());
                            }
                            return;
                        }

                        if ( this->_scene != NULL )
                        {
                            this->_scene->removeItem( this->_horizontalGuide.back());
                        }
                       this->_horizontalGuide.back( )->setLine( qSegment );
                        if ( this->_scene != NULL )
                        {
                            this->_scene->addItem( this->_horizontalGuide.back( ) );
                        }

                    }

                }
        if( _mode == COPY_FACE ){
                    if(CopyFaceSingleton::instance()->getmode()==2){
                        Point_2 clickedPoint = this->snapPoint( event );
                        QRect size_imagetolabel(0,0,1000,1000);
                        QGraphicsScene* currentScene = this->_scene;
                        QList<QGraphicsItem*> allItems = currentScene->items();
                        for(int i=0;i<allItems.count();++i)
                        {
                            if( QGraphicsPixmapItem *p = qgraphicsitem_cast<QGraphicsPixmapItem*>(allItems[i]) )
                                size_imagetolabel = p->pixmap().rect();
                        }
                        QPointF pt = this->_converter( clickedPoint );
                        //initialisation
                        std::vector<Point_2> points_tab;
                        std::vector<Segment_2>segment_tab;
                        std::vector<QLineF> qSegment_tab;
                        std::vector<QPointF> pointf_tab;
                        for(int i=0;i<CopyFaceSingleton::instance()->size();i++){
                            Point_2 gi(pt.x()+CopyFaceSingleton::instance()->getPoint(i).x(), pt.y()+CopyFaceSingleton::instance()->getPoint(i).y());
                             points_tab.push_back(gi);
                             QPointF pointf=this->_converter(gi);
                             pointf_tab.push_back(pointf);
                        }
                        // TODO: make it work for the latest line segment
                        for(int i=0;i<points_tab.size()-1;i++){
                            Segment_2 segment( points_tab[i], points_tab[i+1]);
                            segment_tab.push_back(segment);
                            QLineF qSegment = this->_converter(segment);
                            qSegment_tab.push_back(qSegment);
                        }
                         Segment_2 segment( points_tab[points_tab.size()-1], points_tab[0]);
                           QLineF qSegment = this->_converter(segment);
                           segment_tab.push_back(segment);
                           qSegment_tab.push_back(qSegment);
                        if(this->_horizontalGuide.size()!=0 && this->_horizontalGuide.size()!=points_tab.size())
                            this->_horizontalGuide.clear();
                        if (this->_horizontalGuide.size()==0 ){
                            for(int i=0;i<points_tab.size()-1;i++){
                                QGraphicsLineItem* lineItem = new QGraphicsLineItem( pointf_tab[i].x( ), pointf_tab[i].y( ), pointf_tab[i+1].x( ), pointf_tab[i+1].y( ) );
                                lineItem->setZValue( 100 );
                                QPen pen = lineItem->pen( );
                                pen.setColor( QColor(255, 175, 0));
                                lineItem->setPen( pen );
                                this->_horizontalGuide.push_back( lineItem );
                                if ( this->_scene != NULL )
                                    {
                                    this->_scene->addItem( this->_horizontalGuide.back( ) );
                                }
                            }
                            QGraphicsLineItem* lineItem = new QGraphicsLineItem( pointf_tab[pointf_tab.size()-1].x( ), pointf_tab[points_tab.size()-1].y( ), pointf_tab[0].x( ), pointf_tab[0].y( ) );
                            lineItem->setZValue( 100 );
                            QPen pen = lineItem->pen( );
                            pen.setColor( QColor(255, 175, 0));
                            lineItem->setPen( pen );
                            this->_horizontalGuide.push_back( lineItem );
                            if ( this->_scene != NULL )
                                {
                                this->_scene->addItem( this->_horizontalGuide.back( ) );
                            }



//                            QPointF pg1=this->_converter(g1);
//                            QPointF pg2=this->_converter(g2);
//                             QPointF pg3=this->_converter(g3);
//                              QPointF pg4=this->_converter(g4);
//                            QGraphicsLineItem* lineItem1 = new QGraphicsLineItem( pg1.x( ), pg1.y( ), pg2.x( ), pg2.y( ) );
//                            QGraphicsLineItem* lineItem2 = new QGraphicsLineItem( pg2.x( ), pg2.y( ), pg3.x( ), pg3.y( ) );
//                            QGraphicsLineItem* lineItem3 = new QGraphicsLineItem( pg3.x( ), pg3.y( ), pg4.x( ), pg4.y( ) );
//                            QGraphicsLineItem* lineItem4 = new QGraphicsLineItem( pg4.x( ), pg4.y( ), pg1.x( ), pg1.y( ) );
//                            lineItem1->setZValue( 100 );
//                            lineItem2->setZValue( 100 );
//                            lineItem3->setZValue( 100 );
//                            lineItem4->setZValue( 100 );
//                            QPen pen = lineItem1->pen( );
//                            pen.setColor( QColor(255, 175, 0));
//                            lineItem1->setPen( pen );
//                            lineItem2->setPen( pen );
//                            lineItem3->setPen( pen );
//                            lineItem4->setPen( pen );
//                            this->_horizontalGuide.push_back( lineItem1 );
//                            if ( this->_scene != NULL )
//                            {
//                                this->_scene->addItem( this->_horizontalGuide.back( ) );
//                            }
//                            this->_horizontalGuide.push_back( lineItem2 );
//                            if ( this->_scene != NULL )
//                            {
//                                this->_scene->addItem( this->_horizontalGuide.back( ) );
//                            }
//                            this->_horizontalGuide.push_back( lineItem3 );
//                            if ( this->_scene != NULL )
//                            {
//                                this->_scene->addItem( this->_horizontalGuide.back( ) );
//                            }
//                            this->_horizontalGuide.push_back( lineItem4 );
//                            if ( this->_scene != NULL )
//                            {
//                                this->_scene->addItem( this->_horizontalGuide.back( ) );
//                            }
                        }
                        else
                        {
                            Point_2 sizepoint(size_imagetolabel.width(), size_imagetolabel.height());
                            QPointF psizepoint=this->_converter(sizepoint);
                            int hsize=(int) psizepoint.y();
                            int hsnap=(int)pt.y();
                            int vsize=(int) psizepoint.x();
                            int vsnap=(int)pt.x();
                           if(hsnap<0 || hsnap>hsize || vsnap<0 || vsnap>vsize){
                                if ( this->_scene != NULL )
                                {
                                    for(int i=0;i<points_tab.size();i++)
                                    this->_scene->removeItem( this->_horizontalGuide.at(i));
                                }
                                return;
                            }

                            if ( this->_scene != NULL )
                           {
                               for(int i=0;i<points_tab.size();i++)
                               this->_scene->removeItem( this->_horizontalGuide.at(i));
                           }
                            for(int i=0;i<points_tab.size();i++)
                                this->_horizontalGuide.at(i)->setLine( qSegment_tab[i] );
//                            this->_horizontalGuide.at(1)->setLine( qSegment2 );
//                            this->_horizontalGuide.at(2)->setLine( qSegment3 );
//                            this->_horizontalGuide.at(3)->setLine( qSegment4 );
                            if ( this->_scene != NULL )
                            {
                                for(int i=0;i<points_tab.size();i++)
                                this->_scene->addItem(this->_horizontalGuide.at(i));
                            }

                        }
                    }
                }

        if( _mode == USE_VANISHING ){
            Point_2 clickedPoint = this->snapPoint( event );
            QRect size_imagetolabel(0,0,1000,1000);
            QGraphicsScene* currentScene = this->_scene;
            QList<QGraphicsItem*> allItems = currentScene->items();
            for(int i=0;i<allItems.count();++i){
            if( QGraphicsPixmapItem *p = qgraphicsitem_cast<QGraphicsPixmapItem*>(allItems[i]) )
                size_imagetolabel = p->pixmap().rect();
            }

            QPointF pt = this->_converter( clickedPoint );
            VanishingPoints::instance()->calculate_vanishing_point(QArrangementLabellingVanishingPointsWidget::instance()->GetIndexVanishingPoint());
            if(VanishingPoints::instance()->size()<=QArrangementLabellingVanishingPointsWidget::instance()->GetIndexVanishingPoint()){
                _mode = DEFINE_VANISHING ;
                QArrangementLabellingVanishingPointsWidget::instance()->emitswitch();

                return;
            }
            Point_2 vp0=VanishingPoints::instance()->getVanishingPoints(QArrangementLabellingVanishingPointsWidget::instance()->GetIndexVanishingPoint());
             QPointF vp = this->_converter( vp0 );
              Point_2 g;
             Point_2 d;
              int counter=0;//Permet de savoir combien de points ont étés fixés sur la bordure. 1=>il reste d à fixer; 2=>plus de points à fixer
             if(vp.x()==pt.x()){
                 Point_2 g1(vp.x(),0);
                 g=g1;
                 Point_2 d1(vp.x(), size_imagetolabel.height());
                 d=d1;
             }
             else{
                qreal a=(vp.y()-pt.y())/(vp.x()-pt.x());
                qreal b=(vp.x()*pt.y()-vp.y()*pt.x())/(vp.x()-pt.x());
                qreal limit1=VanishingPoints::instance()->Affine(0,a,b);
                qreal limit2=VanishingPoints::instance()->Affine(size_imagetolabel.width(),a,b);
                int imwidth=size_imagetolabel.width();
                int imheight=size_imagetolabel.height();
                //placement des points
                if (limit1>=0 && limit1<=imheight){
                    Point_2 g1(0, limit1);
                    g=g1;
                    counter+=1;}
                if((-b/a)>0 && (-b/a)<imwidth){
                    if(counter==0){
                        Point_2 g1(-b/a, 0);
                        g=g1;
                        counter+=1;
                    }
                    else if(counter==1){
                        Point_2 d1(-b/a, 0);
                        d=d1;
                        counter+=1;
                    }
                }
                qreal c=(imheight-b)/a;
                if (limit2>=0 && limit2<=imheight){
                    if(counter==0){
                        Point_2 g1(imwidth,  limit2);
                        g=g1;
                        counter+=1;
                    }
                    else if(counter==1){
                        Point_2 d1(imwidth,  limit2);
                        d=d1;
                        counter+=1;
                    }
                }
                if(c>0 && c<imwidth && counter==1){
                    Point_2 d1(c, imheight);
                    d=d1;
                    counter+=1;
                }
             }
            if(counter==2){//On vérifie que l'on passe bien par le dessin car counter<2 => Hors du dessin
                Segment_2 segment( g, d );
                QLineF qSegment = this->_converter( segment );

                    if (this->_horizontalGuide.size()==0 ){
                        QPointF pg=this->_converter(g);
                        QPointF pd=this->_converter(d);
                        QGraphicsLineItem* lineItem = new QGraphicsLineItem( pg.x( ), pg.y( ), pd.x( ), pd.y( ) );
                        lineItem->setZValue( 100 );
                        QPen pen = lineItem->pen( );
                        pen.setColor( QColor(255, 175, 0));
                        lineItem->setPen( pen );
                        this->_horizontalGuide.push_back( lineItem );
                        if ( this->_scene != NULL )
                        {
                            this->_scene->addItem( this->_horizontalGuide.back( ) );
                        }
                    }
                    else
                    {
                        if ( this->_scene != NULL )
                        {
                            this->_scene->removeItem( this->_horizontalGuide.back());
                        }
                       this->_horizontalGuide.back( )->setLine( qSegment );
                        if ( this->_scene != NULL )
                        {
                            this->_scene->addItem( this->_horizontalGuide.back( ) );
                        }

                    }

                        Point_2 sizepoint(size_imagetolabel.width(), size_imagetolabel.height());
                        QPointF psizepoint=this->_converter(sizepoint);
                        int hsize=(int) psizepoint.y();
                        int hsnap=(int)pt.y();
                        int vsize=(int) psizepoint.x();
                        int vsnap=(int)pt.x();
                       if(hsnap<0 || hsnap>hsize || vsnap<0 || vsnap>vsize){
                             if (this->_horizontalGuide.size()==0 )
                                 return;

                           if ( this->_scene != NULL )
                           {
                               this->_scene->removeItem( this->_horizontalGuide.back());
                           }
                   }
            }

                }

        if(_mode==DEFINE_VANISHING){
            if(VanishingPoints::instance()->countervanishing==1){
                //std::cout<<QArrangementLabellingVanishingPointsWidget::instance()->GetIndexVanishingPoint();
                Point_2 clickedPoint = this->snapPoint( event );
                Point_2  pp=VanishingPoints::instance()->GetPointForVanishing();
                Segment_2 segment( pp, clickedPoint );
                QLineF qSegment = this->_converter( segment );
                int index=VanishingPoints::instance()->getIndexSelected();
                if(VanishingPoints::instance()->_VanishingLineGuide.size()==0){
                      QPointF pt = this->_converter( clickedPoint );
                      QPointF px= this->_converter( pp );
                    QGraphicsLineItem* qSegprov=new QGraphicsLineItem(px.x(),px.y(),pt.x(),pt.y());
                    qSegprov->setZValue( 100 );
                    QPen pen = qSegprov->pen( );
                    pen.setColor( QColor(255, 175, 0));
                    qSegprov->setPen( pen );

                    VanishingPoints::instance()->_VanishingLineGuide.push_back(qSegprov);
                }
                else
                    VanishingPoints::instance()->_VanishingLineGuide.back( )->setLine( qSegment );
            }
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
            //WIP test
                QRectF r(0,0,200,200);
                Converter<Kernel> _converttry(r);
                //endWIPTEST
	        QGraphicsScene* currentScene = this->_scene;
	        QList<QGraphicsItem*> allItems = currentScene->items();
	        for(int i=0;i<allItems.count();++i)
            {
		        if( QGraphicsPixmapItem *p = qgraphicsitem_cast<QGraphicsPixmapItem*>(allItems[i]) )
		            size_imagetolabel = p->pixmap().rect();
            }
            QPointF pt = _converttry( clickedPoint );
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
        else if ( _mode == COPY_FACE)
        {    if(CopyFaceSingleton::instance()->getmode()==2){
//                if(CopyFaceSingleton::instance()->getfisrtclick()==1){
//                    CopyFaceSingleton::instance()->firstclick_ok();
//                    return;
//                }
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
                this->_points.clear( );
                for(int i=0;i<CopyFaceSingleton::instance()->size();i++){
                    Point_2 gi(pt.x()+CopyFaceSingleton::instance()->getPoint(i).x(), pt.y()+CopyFaceSingleton::instance()->getPoint(i).y());
                    this->_points.push_back( gi );
                }
                Point_2 g1(pt.x()+CopyFaceSingleton::instance()->getPoint(0).x(), pt.y()+CopyFaceSingleton::instance()->getPoint(0).y());
                this->_points.push_back( g1 );
//                Point_2 g1((int) pt.x()+200, (int) pt.y()+200);
//                 Point_2 g2((int) pt.x()+200, (int) pt.y()-200);
//                  Point_2 g3((int) pt.x()-200, (int) pt.y()-200);
//                   Point_2 g4((int) pt.x()-200, (int) pt.y()+200);
//                this->_points.push_back( g1 );
//                this->_points.push_back( g2 );
//                    this->_points.push_back( g3 );
//                    this->_points.push_back( g4 );
//                   this->_points.push_back( g1 );

                //Curve_2 res( _points.begin( ), _points.end( ) );
                int test=_points.size();
                //_points.clear( );
                switch(event->button( ))
                {   case ::Qt::LeftButton :
                    for(int i=0;i<_points.size();i++){
                         std::vector< Point_2 > _pointsprov;
                         for(int j=0;j<i+1;j++){
                             _pointsprov.push_back(_points.at(j));
                         }
                         if (_pointsprov.size()>1){
                        Curve_2 res( _pointsprov.begin(), _pointsprov.end() );
                        emit generate( CGAL::make_object( res ) );
                         }
                    }
                    _points.clear( );
                    //emit generate( CGAL::make_object( res ) );
                    break;
                    case ::Qt::MiddleButton :
                    if ( this->_scene != NULL )
                    {
                        for(int i=0;i<CopyFaceSingleton::instance()->size();i++)
                        this->_scene->removeItem( this->_horizontalGuide.at(i));
                    }
                     CopyFaceSingleton::instance()->switchmode();
                     _points.clear( );
                     break;
                    case ::Qt::RightButton :
                    //emit generate( CGAL::make_object( res ) );
                    if ( this->_scene != NULL )
                    {
                        for(int i=0;i<CopyFaceSingleton::instance()->size();i++)
                        this->_scene->removeItem( this->_horizontalGuide.at(i));
                    }
                    for(int i=0;i<_points.size();i++){
                         std::vector< Point_2 > _pointsprov;
                         for(int j=0;j<i+1;j++){
                             _pointsprov.push_back(_points.at(j));
                         }
                         if (_pointsprov.size()>1){
                        Curve_2 res( _pointsprov.begin(), _pointsprov.end() );
                        emit generate( CGAL::make_object( res ) );
                         }
                    }
                     _points.clear( );
                    CopyFaceSingleton::instance()->switchmode();
                    break;
                }
            }
            else return;
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
        VanishingPoints::instance()->calculate_vanishing_point(QArrangementLabellingVanishingPointsWidget::instance()->GetIndexVanishingPoint());
        if(VanishingPoints::instance()->size()<=QArrangementLabellingVanishingPointsWidget::instance()->GetIndexVanishingPoint()){
            _mode = DEFINE_VANISHING ;
            QArrangementLabellingVanishingPointsWidget::instance()->emitswitch();

            return;
        }
        Point_2 vp0=VanishingPoints::instance()->getVanishingPoints(QArrangementLabellingVanishingPointsWidget::instance()->GetIndexVanishingPoint());
         QPointF vp = this->_converter( vp0 );
        //Ici, il faut un moyen de stocker le point sous forme de singleton et de type compatiple avec les autres points
        //Ensuite, on calcul l'equation de la droite qui passe entre ce point et le point cliqué
         Point_2 g;
         Point_2 d;
          int counter=0;//Permet de savoir combien de points ont étés fixés sur la bordure. 1=>il reste d à fixer; 2=>plus de points à fixer
         if(vp.x()==pt.x()){
             Point_2 g1(vp.x(),0);
             g=g1;
             Point_2 d1(vp.x(), size_imagetolabel.height());
             d=d1;
         }
         else{
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



            //On place le point à gauche
            if (limit1>=0 && limit1<=imheight){
                Point_2 g1(0, limit1);
                g=g1;
                counter+=1;}
            if((-b/a)>0 && (-b/a)<imwidth){
                if(counter==0){
                    Point_2 g1(-b/a, 0);
                    g=g1;
                    counter+=1;
                }
                else if(counter==1){
                    Point_2 d1(-b/a, 0);
                    d=d1;
                    counter+=1;
                }
            }

           //on place le point à droite
            qreal c=(imheight-b)/a;
            if (limit2>=0 && limit2<=imheight){
                if(counter==0){
                    Point_2 g1(imwidth,  limit2);
                    g=g1;
                    counter+=1;
                }
                else if(counter==1){
                    Point_2 d1(imwidth,  limit2);
                    d=d1;
                    counter+=1;
                }
            }
            if(c>0 && c<imwidth && counter==1){
                Point_2 d1(c, imheight);
                d=d1;
                counter+=1;
            }
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
        {   //WIP int index=VanishingPoints::instance()->getIndexSelected();
            if(VanishingPoints::instance()->vide(0)==true){
                QArrangementLabellingVanishingPointsWidget::instance()->addVanishingPoint();
            }
            int index=QArrangementLabellingVanishingPointsWidget::instance()->GetIndexVanishingPoint();
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
                pen.setColor(QColor(255, 175, 0));

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
                VanishingPoints::instance()->addVanishingEdges(point1.x(),point1.y(),point2.x(),point2.y(),index);
                this->_points.push_back( clickedPoint );

                // #37 (https://github.com/oliviertournaire/QLabelling/issues/37): This is where the crash happens when the user click consecutively the same point
               std::vector<Point_2>  pp;
               pp.push_back(VanishingPoints::instance()->GetPointForVanishing());

                VanishingPoints::instance()->countervanishing=0;

                switch(event->button( ))
                {   case ::Qt::LeftButton :
                    this->_points.clear( );
                    break;
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
                        pen.setColor( QColor(255, 175, 0) );
                        lineItem->setPen( pen );
                        VanishingPoints::instance()->_VanishingLineGuide.push_back( lineItem ); // Ajout à l'objet Polyline de ce nouveau segment
                        if ( this->_scene != NULL )
                        {
                            this->_scene->addItem( VanishingPoints::instance()->_VanishingLineGuide.back( ) ); // Ajout à la scène du dernier segment (celui qui vient d'être ajouté)
                        }
                        //WIP auto calculate every time
                        int num=VanishingPoints::instance()->EdgesSize(index);
                        if(num>1)
                            VanishingPoints::instance()->calculate_vanishing_point(index);
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
     std::vector< QGraphicsLineItem* > _horizontalGuide;
};

} // namespace Qt
} // namespace CGAL

#endif // CGAL_QT_GRAPHICS_VIEW_SEGMENT_INPUT_H
