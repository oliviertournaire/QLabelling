#ifndef VANISHINGPOINTS_H
#define VANISHINGPOINTS_H

#include "utilities/Singleton.hpp"
#include "arrangement/ArrangementTypes.h"
#include <QGraphicsLineItem>
#include <QtCore/qmath.h>

struct VanishingEdge{
    qreal x1;
    qreal x2;
    qreal y1;
    qreal y2;
};

class VanishingPoints : public Singleton<VanishingPoints>
{
public:
     std::vector< QGraphicsLineItem* > _VanishingLineGuide;
     int countervanishing;

    VanishingPoints() {}
    //rajoute un vanishingpoint
    void addVanishingPoint(const Arr_pol_point_2& p)
    {
        _vanishingPoints.push_back(p);
    }
    //Attribut une valeur à un vanishing point deja existant à partir d'un point
    void setVanishingPoints(const Arr_pol_point_2& p, unsigned int index)
    {
        if( index>=_vanishingPoints.size() )
            return;

        _vanishingPoints[index] = p;
    }
    //Attribut une valeur à un point de fuite à partir de 2 doubles
    void setVanishingPoints(double x, double y, unsigned int index)
    {
        if( index>=_vanishingPoints.size() )
            return;

        Arr_pol_point_2 new_vanishing_point(x,y);
        _vanishingPoints[index] = new_vanishing_point;
    }
    //Obtient le point de fuite numero index
    Arr_pol_point_2 getVanishingPoints(unsigned int index)
    {
        if( index>=_vanishingPoints.size() )
        {
            throw std::out_of_range("Bad index value");
        }

        return _vanishingPoints[index];
    }
    //Taille du singleton
    unsigned int size()
    {
        return (unsigned int)_vanishingPoints.size();
    }

    unsigned int EdgesSize(unsigned int index){//taille d'un vecteur de lignes de fuite
        return (unsigned int)_vanishingEdges[index].size();
    }

    unsigned int EdgesVectorSize(){ //taille du vecteur de l'ensemble des lignes de fuites
        return (unsigned int)_vanishingEdges.size();
    }


    //calcul un point de la droite
    qreal Affine(qreal x,qreal a,qreal b){
        return a*x+b;
    };
    //Ajouter une nouvelle ligne à un vecteur de lignes de fuites
    void addVanishingEdge(const VanishingEdge& e,unsigned int index)
    {
        _vanishingEdges[index].push_back(e);
    };
    //Ajouter un nouveau vecteur de lignes de fuites
    void addVanishingEdges(const VanishingEdge& e)
    {     std::vector<VanishingEdge> edges;
          edges.push_back(e);
        _vanishingEdges.push_back(edges);
    };
    void addVanishingEdges(qreal x1,qreal y1,qreal x2, qreal y2,unsigned int index)
    {     std::vector<VanishingEdge> edges;
          VanishingEdge newedge;
          newedge.x1=x1;
            newedge.x2=x2;
            newedge.y1=y1;
            newedge.y2=y2;
            if (_vanishingEdges.size()<=index || _vanishingEdges.size()==0){

              edges.push_back(newedge);
              _vanishingEdges.push_back(edges);
            }
            else
                _vanishingEdges[index].push_back(newedge);


    };

    VanishingEdge getVanishingEdges(unsigned int index,unsigned int index2)
    {
        if( index>=_vanishingEdges.size() )
        {
            throw std::out_of_range("Bad index value");
        }
        if(index2>=_vanishingEdges[index].size())
        {
            throw std::out_of_range("Bad index value");
        }
        return _vanishingEdges[index][index2];
    }
    int getIndexSelected(){
        return _indexSelected;
    }
    void setIndexSelected(unsigned int index){
        _indexSelected=index;
    }
    bool vide(unsigned int index){
        if(index>=_vanishingEdges.size())
            return true;
        else
            return false;
    }
    void SetPointForVanishing(Arr_pol_point_2 p){
       _pointforvanishing=p;
    }
    Arr_pol_point_2 GetPointForVanishing(){
        return _pointforvanishing;
    }


    void calculate_vanishing_point(unsigned int index){
        //TODO : utiliser un autre algorithme si celui ci n'est pas suffisament précis
        //on verifie qu'il y ai au moins 2 droites
        //n : nombre de lignes de fuites définissant le point de fuite
        if(_vanishingEdges.size()<=index)
            return;
        unsigned int n = (unsigned int)_vanishingEdges[index].size();
        if (n<2)
            return;
        std::vector<qreal> a;
        std::vector<qreal> b;
        //On calcule les coeff ai et bi de chaque droite fi(x)=ai*x+bi
        for(unsigned int i=0;i<n;i++){
            qreal x1=_vanishingEdges[index][i].x1;
            qreal x2=_vanishingEdges[index][i].x2;
            qreal y1=_vanishingEdges[index][i].y1;
            qreal y2=_vanishingEdges[index][i].y2;

            qreal ai=(y1-y2)/(x1-x2);
            qreal bi=(x1*y2-x2*y1)/(x1-x2);
            a.push_back(ai);
            b.push_back(bi);
        }
        //On définie les termes sum(bi)/n et sum(ai)/n
        qreal bsum=0;
        qreal asum=0;
        for(unsigned int i=0;i<n;i++){
            bsum=bsum+b[i]/n;
            asum=asum+a[i]/n;
        }
        //On calcul les x potentiels du point de fuites
        std::vector<qreal> xopt;
        for(unsigned int i=0;i<n;i++){
            if(a[i]!=asum){
                qreal xi=-(b[i]-bsum)/(a[i]-asum);
                xopt.push_back(xi);
            }
            else{
                qreal xi=-(b[i]-bsum)/(a[i]*1.1-asum);
                xopt.push_back(xi);
            }
        }
        //On cherche a présent le minimum
        qreal xmin=xopt[0];
        qreal distancemin;
        for(unsigned int i=0;i<n;i++){
            qreal distance=0;
            qreal mid_sum=0;
            for(unsigned int k=0;k<n;k++){
               mid_sum=mid_sum+(a[k]*xopt[i]+b[k])/n;
            for(unsigned int j=0;j<n;j++){
                  qreal absolute=a[j]*xopt[i]-mid_sum+b[j];
                  if(absolute<0)
                      absolute=-absolute;
                  distance=distance+absolute/(qSqrt(1+a[j]*a[j]));
                }
                if(i==0)
                    distancemin=distance;
                if(distance<distancemin){
                    distancemin=distance;
                    xmin=xopt[i];
                }
            }
        }
        //fin de la boucle de calcul, il reste a calculer y
        qreal ymin=0;
        for(unsigned int i=0;i<n;i++){
            ymin=ymin+(a[i]*xmin+b[i])/n;
        }
        //On défini alors le point de fuite
         Arr_pol_point_2 new_vanishing_point(xmin,ymin);
         Arr_pol_point_2 pointzero(0,0);
        while(_vanishingPoints.size()<index)
            addVanishingPoint(pointzero);
        if(_vanishingPoints.size()==index)
             addVanishingPoint(new_vanishing_point);
        else
            setVanishingPoints(new_vanishing_point,index);
    }
    void erase(unsigned int index){
        _vanishingEdges.erase(_vanishingEdges.begin()+index);
        if (size()>index)
        _vanishingPoints.erase(_vanishingPoints.begin()+index);
    }
    void clearEdgesVector(){
        _vanishingEdges.clear();
        if(size()==1)
        _vanishingPoints.erase(_vanishingPoints.begin());
    }

private:
    std::vector<std::vector<VanishingEdge> > _vanishingEdges;
    std::vector<Arr_pol_point_2> _vanishingPoints;
    int _indexSelected;
    Arr_pol_point_2 _pointforvanishing;

};

#endif // VANISHINGPOINTS_H
