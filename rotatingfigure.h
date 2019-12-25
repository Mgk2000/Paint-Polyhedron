#ifndef ROTATINGFIGURE_H
#define ROTATINGFIGURE_H
#include "drawingobject.h"
class MainWidget;
class RotatingFigure : public DrawingObject
{
public:
    RotatingFigure(MainWidget * mw, bool _little);
    virtual int pick(float mx, float my, int icolor) = 0;
    virtual ~RotatingFigure();
    virtual void init() =0;
    virtual void setData(const uchar* data) = 0;
    virtual int validColorsCount (RotatingFigure* lf) =0;

    bool needsCellDraw;
    bool needsFullDraw;
protected:
    MainWidget * mainWidget;
    QVector3D rotatePoint(const QVector3D &v) const;
    bool little;

};

#endif // ROTATINGFIGURE_H
