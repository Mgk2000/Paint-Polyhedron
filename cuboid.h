#ifndef CUBOID_H
#define CUBOID_H
#include "polyhedron.h"

class Cuboid: public Polyhedron
{
public:
    Cuboid(MainWidget * mw);
};
class LittleCuboid : public LittlePolyhedron
{
public:
    LittleCuboid (Cuboid* bigCuboid);
};
#endif // CUBOID_H
