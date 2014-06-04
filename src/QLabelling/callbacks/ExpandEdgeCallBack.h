#ifndef EXPANDEDGECALLBACK_H
#define EXPANDEDGECALLBACK_H

#include "Callback.h"

//#include <QEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <CGAL/Qt/Converter.h>
//WIP a supprimer
#include <CGAL/Arrangement_with_history_2.h>
//end WIP
#include "CurveGraphicsItem.h"
#include "Utils.h"
#include "VanishingPoints.h"
#include <CGAL/Qt/GraphicsViewInput.h>
#include <CGAL/Arrangement_2.h>

/**
   Handles expantion of arrangement edges selected from the scene.

   The template parameter is a CGAL::Arrangement_with_history_2 of some type.
*/
class emitexpand : public Singleton<emitexpand>{
    friend class Singleton<emitexpand>;
    Q_OBJECT
public:
    emitexpand(){}
signals:
    void emitexpandedge(CGAL::Object);
public slots:
    void emitexpandedgeslot(CGAL::Object obj){
        emit emitexpandedge(obj);
    }
};

template < typename Arr_ >
class ExpandEdgeCallBack : public CGAL::Qt::Callback
{

public:
    typedef Arr_ Arrangement;
    typedef typename Arrangement::Halfedge_const_handle   Halfedge_const_handle;
    typedef typename Arrangement::Halfedge_handle         Halfedge_handle;
     typedef typename Arrangement::Halfedge               Halfedge;
     typedef typename Arrangement::Vertex_handle         Vertex_handle;
        typedef typename Arrangement::Vertex_iterator         Vertex_iterator;
    typedef typename Arrangement::Halfedge_iterator       Halfedge_iterator;
    typedef typename Arrangement::Geometry_traits_2       Traits;
    typedef typename Arrangement::Curve_handle            Curve_handle;
    typedef typename Arrangement::Originating_curve_iterator
    Originating_curve_iterator;
    typedef typename Arrangement::Induced_edge_iterator   Induced_edge_iterator;
    typedef typename Traits::X_monotone_curve_2           X_monotone_curve_2;
    typedef typename ArrTraitsAdaptor< Traits >::Kernel   Kernel;
    typedef typename Kernel::Point_2                      Point;
    typedef typename Kernel::Segment_2                    Segment;
    typedef typename Traits::Curve_2                        Curve_2;
//WIPWIP
//    typedef CGAL::Exact_predicates_exact_constructions_kernel Kernelbis;
//    typedef CGAL::Arr_segment_traits_2<Kernelbis> Traits_2bis;
//    typedef Traits_2bis::Point_2 Point_2bis;
//    typedef Traits_2bis::X_monotone_curve_2 Segment_2bis;
//    typedef CGAL::Arrangement_2<Traits_2bis> Arrangement_2bis;
//    typedef Arrangement_2bis::Halfedge_handle Halfedge_handlebis;
//    typedef Arrangement_2bis::Vertex_handle Vertex_handlebis;
    //endWIP
    ExpandEdgeCallBack( Arrangement* arr_, QObject* parent_ );
    void setScene( QGraphicsScene* scene_ );
    QGraphicsScene* getScene( ) const;
    void reset( );
signals:
    void emitexpandedge(CGAL::Object);
protected:
    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
    void highlightNearestCurve( QGraphicsSceneMouseEvent *event );
    std::vector< Point > _points;

    Compute_squared_distance_2< Traits > squaredDistance;
    CGAL::Qt::Converter< Kernel > convert;
    QGraphicsScene* scene;
    CGAL::Qt::CurveGraphicsItem< Traits >* highlightedCurve;
    Curve_2* addcurve;
    Arrangement* arr;
   // Arrangement_2bis arrbis;
    Halfedge_handle expandableHalfedge;
    //Halfedge_handlebis expandableHalfedgebis;
    Halfedge expandableHalfedgetrue;
}; // class ExpandEdgeCallBack

/*! Constructor */
template < typename Arr_ >
ExpandEdgeCallBack< Arr_ >::
ExpandEdgeCallBack( Arrangement* arr_, QObject* parent_ ) :
    CGAL::Qt::Callback( parent_ ),
    scene( NULL ),
    highlightedCurve( new CGAL::Qt::CurveGraphicsItem< Traits >( ) ),
    addcurve(new Curve_2()),

    arr( arr_ )
{
    QObject::connect( this, SIGNAL( modelChanged( ) ),
                      this->highlightedCurve, SLOT( modelChanged( ) ) );
//    QObject::connect( this, SIGNAL( modelChanged( ) ),
//                      this->addcurve, SLOT( modelChanged( ) ) );
}

template < typename Arr_ >
void ExpandEdgeCallBack< Arr_ >::setScene( QGraphicsScene* scene_ )
{
    this->scene = scene_;
    this->highlightedCurve->setScene( scene_ );
    if ( this->scene )
    {
        this->scene->addItem( this->highlightedCurve );
    }
}

template < typename Arr_ >
QGraphicsScene* ExpandEdgeCallBack< Arr_ >::getScene( ) const
{
    return this->scene;
}

template < typename Arr_ >
void ExpandEdgeCallBack< Arr_ >::reset( )
{
    this->highlightedCurve->clear( );
    this->addcurve->clear();
    this->expandableHalfedge = Halfedge_handle( );
    this->expandableHalfedgetrue=Halfedge();
    emit modelChanged( );
}

template < typename Arr_ >
void
ExpandEdgeCallBack<Arr_>::mousePressEvent(QGraphicsSceneMouseEvent* /* event */)
{QRect size_imagetolabel(0,0,1000,1000);
    if ( this->expandableHalfedge == Halfedge_handle( ) )
    {
        return;
    }
    QGraphicsScene* currentScene = this->scene;
    if(currentScene)
    {
        QList<QGraphicsItem*> allItems = currentScene->items();
        for(int i=0;i<allItems.count();++i)
            if( QGraphicsPixmapItem *p = qgraphicsitem_cast<QGraphicsPixmapItem*>(allItems[i]) )
                size_imagetolabel = p->pixmap().rect();
    }
    //WIP TODO:
    //1 - vérifier dans quel sens on peut prolonger (=> conditions au bord, être sur que l'edge n'est déjà pas au bord d'un coté)
    //2 - si on peut prologer d'1 ou 2 coté, prendre les inofrmations sur l'edge (x et y des extrémités)


    Halfedge_iterator nearestHei;




        Vertex_iterator source = expandableHalfedge->source( );
        Vertex_iterator target = expandableHalfedge->target( );
//CGAL::Arr_vertex_base<CGAL::Point_2<CGAL::Epeck>>;
       const Point p1=source->point();
         const Point p2=target->point();

         //WIP WIP récupéré d'une autre fonction
         QPointF vp = this->convert( p1);
         QPointF pt = this->convert( p2 );
        //Ici, il faut un moyen de stocker le point sous forme de singleton et de type compatiple avec les autres points
        //Ensuite, on calcul l'equation de la droite qui passe entre ce point et le point cliqué
         Point g;
         Point d;
          int counter=0;//Permet de savoir combien de points ont étés fixés sur la bordure. 1=>il reste d à fixer; 2=>plus de points à fixer
          int vpx=vp.x();
          int ptx=pt.x();
         if(vp.x()==pt.x()){
             Point g1(vp.x(),0);
             g=g1;
             Point d1(vp.x(), size_imagetolabel.height());
             d=d1;
         }
         else{
            qreal a=(vp.y()-pt.y())/(vp.x()-pt.x());
            qreal b=(vp.x()*pt.y()-vp.y()*pt.x())/(vp.x()-pt.x());
            //ATTENTION : ne prend pas en compte la division par 0
            //on trouve a=(y2-y1)/(x2-x1) et b=(x1y2-x2y1)/(x2-x1)
            //on réalise ensuite un test pour voir sur quelles bordures se trouve le point : on teste donc si f(0) est compris entre 0 et size_imagetolabel.height() :
            //si oui, on prend Point g(0,(int)f(0));
            //si non, on calcul x pour y=0 => Point g((int)-b/a,0)
            //Prévoir tous les risques avec un autre cas : hors cadre => on trace rien
            //On applique les mêmes étapes pour le second point



            qreal limit1=VanishingPoints::instance()->Affine(0,a,b);
            qreal limit2=VanishingPoints::instance()->Affine(size_imagetolabel.width(),a,b);
            int imwidth=size_imagetolabel.width();
            int imheight=size_imagetolabel.height();



            //On place le point à gauche
            if (limit1>=0 && limit1<=imheight){
                Point g1(0, limit1);
                g=g1;
                counter+=1;}
            if((-b/a)>0 && (-b/a)<imwidth){
                if(counter==0){
                    Point g1(-b/a, 0);
                    g=g1;
                    counter+=1;
                }
                else if(counter==1){
                    Point d1(-b/a, 0);
                    d=d1;
                    counter+=1;
                }
            }

           //on place le point à droite
            qreal c=(imheight-b)/a;
            if (limit2>=0 && limit2<=imheight){
                if(counter==0){
                    Point g1(imwidth,  limit2);
                    g=g1;
                    counter+=1;
                }
                else if(counter==1){
                    Point d1(imwidth,  limit2);
                    d=d1;
                    counter+=1;
                }
            }
            if(c>0 && c<imwidth && counter==1){
                Point d1(c, imheight);
                d=d1;
                counter+=1;
            }
         }
        if(counter==2){//On vérifie que l'on passe bien par le dessin car counter<2 => Hors du dessin
            this->_points.push_back( g );
            this->_points.push_back( d );
            const Curve_2 res( this->_points.begin( ), this->_points.end( ) );
            CGAL::insert( *(this->arr), res);
           //Segment_2 s5(Point(1, 3), Point(5, 3));
           //Vertex_handle v2 = expandableHalfedge->target();
           //Halfedge_handle e2=arr->insert_from_right_vertex (s5, v2);
            //X_monotone_curve_2 res2=res;


            this->_points.clear( );
           // CGAL::Object obj=CGAL::make_object(res);
//            QObject::connect(emitexpand::instance(),SIGNAL(emitexpandedge(CGAL::Object)),
//                             emitexpand::instance(),SLOT(emitexpandedgeslot(CGAL::Object)));
//           // emit emitexpandedge( obj );


//        if ( source->degree( ) != 2 && target->degree( ) != 2 )
//        { // then this halfedge has no mergeable neighbors
//            continue;
//        }
//        Halfedge_handle h1 = hei->prev( );
//        Halfedge_handle h2 = hei->next( );
//        if ( (! this->arr->are_mergeable( hei, h1 ) ) &&
//             (! this->arr->are_mergeable( hei, h2 ) ) )
//        {
//            continue;
//        }

//        X_monotone_curve_2 curve = hei->curve( );
//        double dist = CGAL::to_double( this->squaredDistance( p, curve ) );
//        if ( noneFound || dist < minDist )
//        {
//            noneFound = false;
//            minDist = dist;
//            nearestHei = hei;
//        }
    }






    //WIP c'est là que tout se passe; à changer
//    bool deleteOriginatingCurve = 1;
//    if ( deleteOriginatingCurve )
//    {
//        Originating_curve_iterator it =
//                this->arr->originating_curves_begin( this->expandableHalfedge );
//        Originating_curve_iterator it_end =
//                this->arr->originating_curves_end( this->expandableHalfedge );
//        while ( it != it_end )
//        {
//            Originating_curve_iterator temp = it;
//            ++temp;
//            CGAL::remove_curve( *(this->arr), it );
//            it = temp;
//        }
//    }
//    else
//    {
//        CGAL::remove_edge( *(this->arr), this->expandableHalfedge->curve( ) );
//        this->arr->remove_edge( this->expandableHalfedge );
//    }

    this->reset( );
}

template < typename Arr_ >
void ExpandEdgeCallBack< Arr_ >::
mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
    this->highlightNearestCurve( event );
}

template < typename Arr_ >
void
ExpandEdgeCallBack< Arr_ >::
highlightNearestCurve( QGraphicsSceneMouseEvent* event )
{
    // find the nearest curve to the cursor to be the new highlighted curve
    Point p = this->convert( event->scenePos( ) );
    //bool isFirst = true;
    //double minDist = 0.0;
    //Halfedge_iterator nearestHei;

    Find_nearest_edge< Arr_ > findNearestEdge( this->arr );
    findNearestEdge.setScene( this->scene );
    Halfedge_const_handle nearestEdge = findNearestEdge( p );
    this->expandableHalfedge = this->arr->non_const_handle( nearestEdge );

    // now 'expandableHalfedge' holds the closest halfedge to the point of the mouse
    //this->expandableHalfedge = nearestHei;
    //if ( isFirst )
    if ( this->expandableHalfedge == Halfedge_handle( ) )
    {
        // std::cout << "no curve found" << std::endl;
        return;
    }

    // create a curve graphics item and add it to the scene
    //bool deleteOriginatingCurve = 1;
    this->highlightedCurve->clear( );
//    if ( deleteOriginatingCurve )
//    { // highlight the originating curve
//        Originating_curve_iterator ocit, temp;
//        ocit = this->arr->originating_curves_begin( this->expandableHalfedge );
//        while (ocit != this->arr->originating_curves_end(this->expandableHalfedge))
//        {
//            temp = ocit;
//            ++temp;

//            Curve_handle ch = ocit;
//            Induced_edge_iterator itr;
//            for ( itr = this->arr->induced_edges_begin( ch );
//                  itr != this->arr->induced_edges_end( ch );
//                  ++itr )
//            {
//                X_monotone_curve_2 curve = (*itr)->curve( );
//                this->highlightedCurve->insert( curve );
//            }
//            ocit = temp;
//        }
//    }
//    else
   // { // highlight just the edge
    //WIP on veut juste highlighter l'edge ici;
        this->highlightedCurve->insert( this->expandableHalfedge->curve( ) );
    //}

    emit modelChanged( );
}




#endif // EXPANDEDGECALLBACK_H
