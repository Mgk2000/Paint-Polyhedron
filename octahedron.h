#ifndef OCTAHEDRON_H
#define OCTAHEDRON_H
#include "polyhedron.h"

class Octahedron : public Polyhedron
{
public:
    Octahedron(MainWidget * mw);
};
class LittleOctahedron : public LittlePolyhedron
{
public:
    LittleOctahedron(Octahedron* bigOct);
};
#endif // OCTAHEDRON_H
