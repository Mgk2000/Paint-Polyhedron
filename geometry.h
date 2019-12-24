#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <QVector3D>
#include <QVector2D>

void pointInParallelogram(QVector3D& A,QVector3D& B, QVector3D& C,
                  QVector3D& D,QVector3D& M, QVector3D& Res, bool* inside);
#endif // GEOMETRY_H
