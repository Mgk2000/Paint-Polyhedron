#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H
#include "polyhedron.h"

class Tetrahedron : public Polyhedron
{
public:
    Tetrahedron(MainWidget * mw);
};

class LittleTetrahedron : public LittlePolyhedron
{
public:
    LittleTetrahedron(Tetrahedron * bigTet);
};
#endif // TETRAHEDRON_H
