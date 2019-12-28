#ifndef ICOSAHEDRON_H
#define ICOSAHEDRON_H
#include "polyhedron.h"


class Icosahedron : public Polyhedron
{
public:
    Icosahedron(MainWidget * mw);
};
class LittleIcosahedron : public LittlePolyhedron
{
public:
    LittleIcosahedron(Icosahedron* bigIcos);
};

#endif // ICOSAHEDRON_H
