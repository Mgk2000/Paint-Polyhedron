#include "polyhedron.h"

Polyhedron::Polyhedron(MainWidget * mw, bool _little) : RotatingFigure (mw, _little)
{

}
void Polyhedron::init()
{

}
void Polyhedron::draw()
{

}
void Polyhedron::setData(const uchar *data)
{

}
int Polyhedron::pick(float mx, float my, int icolor)
{

}

void Polyhedron::fillData()
{

}

int Polyhedron::validColorsCount(RotatingFigure *lf)
{
    return 0;
}

void Polyhedron::initGL(QOpenGLShaderProgram *prog)
{
    DrawingObject::initGL(prog,1);
}
void Polyhedron::setNcells(int nc)
{

}
Vertex::Vertex(float _x, float _y, float _z)
{
    vertex = QVector3D(_x, _y, _z);
}

_Face::_Face()
{

}
