#ifndef COPYFACESINGLETON_H
#define COPYFACESINGLETON_H


#include "utilities/Singleton.hpp"
#include "arrangement/ArrangementTypes.h"
#include <QGraphicsLineItem>


class CopyFaceSingleton : public Singleton<CopyFaceSingleton>
{
public:
    CopyFaceSingleton();
    void clear(){
        if (_copyFaceVertex.size()!=0)
            _copyFaceVertex.clear();
    }
    void addPoint(Arr_pol_point_2 p){
        _copyFaceVertex.push_back(p);
    }
    int size(){
        return _copyFaceVertex.size();
    }
    void createRelativeVertex(Arr_pol_point_2 p){
        if(_copyFaceRelativeVertex.size()!=0)
            _copyFaceRelativeVertex.clear();
        for(int i=0;i<_copyFaceVertex.size();i++){
            CGAL::Lazy_exact_nt<CGAL::Gmpq> lazyxi=_copyFaceVertex[i].x()-p.x();
            CGAL::Lazy_exact_nt<CGAL::Gmpq> lazyyi=_copyFaceVertex[i].y()-p.y();

            qreal xi = CGAL::to_double(lazyxi);
            qreal yi= CGAL::to_double(lazyyi);

            //qreal xi=_copyFaceVertex[i].x()-p.x();
            //qreal yi=_copyFaceVertex[i].y()-p.y();
            QPointF pi(xi,yi);
            _copyFaceRelativeVertex.push_back(pi);
        }

    }
    int _mode;
    int getmode(){
        return _mode;
    }

    void setCopyFace(){
        _mode=1;
        _firstclick=0;
    }
    int getfisrtclick(){
        return _firstclick;
    }
    void firstclick_ok(){
        //return to 0 once you've changed mode
        _firstclick=0;
    }

    void switchmode(){
        if(_mode==1){
            _firstclick=1;
            _mode=2;

        }
       else{
            _firstclick=1;
            _mode=1;

        }
    }
    QPointF getPoint(int index){
        return _copyFaceRelativeVertex.at(index);
    }

private :
    std::vector<Arr_pol_point_2> _copyFaceVertex;
    std::vector<QPointF> _copyFaceRelativeVertex;
    int _firstclick;
};

#endif // COPYFACESINGLETON_H
