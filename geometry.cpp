#include "geometry.h"
#include <QtMath>
float distToLine(const QVector2D& p, const QVector2D& a,const QVector2D& b)
{
    return p.distanceToLine(a, (b-a).normalized());
}
void pointInParallelogram(QVector3D &A, QVector3D &B, QVector3D& C,
            QVector3D &D, QVector3D &M, QVector3D& res, bool *inside)
{
    float dw = 0.1;
/*    if (qFabs(A.x() - B.x()) < dw || qFabs(A.y() - B.y()) ||
        qFabs(A.x() - D.x()) < dw || qFabs(A.y() - D.y()))
    { *inside = false; return;}

float Alpha = (B.x() - A.x()) / (B.y()-A.y());
float Beta = A.x() - A.y() * (B.x() - A.x()) / (B.y() - A.y());
float Gamma = (D.x() - A.x()) / (D.y() - A.y());
float Delta = M.x() - M.y() * (D.x() - A.x())/(D.y() - A.y());
float py = (Delta - Beta) / (Alpha - Gamma);
float px = A.x() + (B.x() - A.x()) * (py-A.y()) / (D.y() - A.y());
float mx= px;
float my = A.y() + M.y() - py;
res.setX(mx / (B.x() - A.x()));
res.setY(my / (D.y() - A.y()));
*inside = true;*/
/*    QVector2D v1 (-0.1,-1);
    QVector2D v2 (0.1,-1);
    QVector2D v3(-13,1);
    QVector2D v4(-1,1);
    QVector2D v5(7,1);
    float f1 = distToLine(v3,v1,v2);
    float f2 = distToLine(v4,v1,v2);
    float f3 = distToLine(v5,v1,v2);*/

    QVector2D AA = A.toVector2D();
    QVector2D BB = B.toVector2D();
    QVector2D DD = D.toVector2D();
    QVector2D CC = C.toVector2D();
    float h = distToLine(DD, AA,BB);
    if (h<dw)     { *inside = false; return;}
    float w = distToLine(BB, AA,DD);
    if (w<dw)     { *inside = false; return;}
    QVector2D MM = M.toVector2D();
    float x = distToLine(MM, AA,DD);
    if (x>=w) { *inside = false; return;}
    float xx = distToLine(MM, BB, CC);
    if (xx>=w) { *inside = false; return;}
    float y = distToLine(MM, AA,BB);
    if (y>=h){ *inside = false; return;}
    float yy = distToLine(MM, CC, DD);
    if (yy>=h){ *inside = false; return;}
    res.setX(x/w);
    res.setY(y/h);
    *inside = true;
}

