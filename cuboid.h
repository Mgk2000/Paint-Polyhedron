#ifndef CUBOID_H
#define CUBOID_H
#include "polyhedron.h"

class Cuboid: public Polyhedron
{
public:
    Cuboid(MainWidget * mw, bool div3 = false);
protected:
#ifdef WIN32
    bool hasCubicSymmetry() const override {return true;}
#endif

};
class LittleCuboid : public LittlePolyhedron
{
public:
    LittleCuboid (Cuboid* bigCuboid);
};
#endif // CUBOID_H
