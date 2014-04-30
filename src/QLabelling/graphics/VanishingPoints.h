#ifndef VANISHINGPOINTS_H
#define VANISHINGPOINTS_H

#include "utilities/Singleton.hpp"
#include "arrangement/ArrangementTypes.h"

class VanishingPoints : public Singleton<VanishingPoints>
{
public:
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
        return _vanishingPoints.size();
    }
    //calcul un point de la droite
    qreal Affine(qreal x,qreal a,qreal b){
        return a*x+b;
    };

private:
    std::vector<Arr_pol_point_2> _vanishingPoints;
};

#endif // VANISHINGPOINTS_H
