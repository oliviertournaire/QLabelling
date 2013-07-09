#include "ArrangementTypes.h"

#include <iostream>

std::ostream& operator <<(std::ostream &o, const QColor &c)
{
    o << c.red() << " " << c.green() << " " << c.blue() << " " << c.alpha();
    return o;
}

std::istream& operator>>(std::istream &i, QColor &c)
{
    int r, g, b, a;
    i >> r >> g >> b >> a;
    c.setRed(r);
    c.setGreen(g);
    c.setBlue(b);
    c.setAlpha(a);
    return i;
}

std::ostream& operator<<(std::ostream &o, const Face_with_color &f)
{
    o << f.label().toStdString() << " " << f.color();
    return o;
}

std::istream& operator>>(std::istream &i, Face_with_color &f)
{
    QColor c;
    std::string l;
    i >> l >> c;
    f.set_label(QString::fromStdString(l));
    f.set_color(c);
    return i;
}