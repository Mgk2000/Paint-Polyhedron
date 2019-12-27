#include "rotatingfigure.h"
#include "mainwidget.h"
RotatingFigure::RotatingFigure(MainWidget * mw, bool _little) : needsCellDraw(false), needsFullDraw(true),

    mainWidget(mw), little(_little)
{

}

RotatingFigure::~RotatingFigure()
{

}

QVector3D RotatingFigure::rotatePoint(const QVector3D &v) const
{
    return mainWidget->rotatePoint(v);
}

float RotatingFigure::sign (float p1x, float p1y, float p2x, float p2y, float p3x, float p3y)
{
    return (p1x - p3x) * (p2y - p3y) - (p2x - p3x) * (p1y - p3y);
}

bool RotatingFigure::PointInTriangle (float ptx, float pty, float v1x, float v1y,
           float v2x, float v2y,  float v3x, float v3y)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(ptx, pty, v1x, v1y, v2x, v2y);
    d2 = sign(ptx, pty, v2x, v2y, v3x, v3y);
    d3 = sign(ptx, pty, v3x, v3y, v1x, v1y);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}
