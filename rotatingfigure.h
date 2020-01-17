#ifndef ROTATINGFIGURE_H
#define ROTATINGFIGURE_H
#include "drawingobject.h"
class MainWidget;
struct DataFileInfo;
class RotatingFigure : public DrawingObject
{
public:
    RotatingFigure(MainWidget * mw, bool _little);
    virtual int pick(float , float , int ) {return 0;}
    virtual ~RotatingFigure();
    virtual void init() =0;
    virtual void setData(const uchar* data) = 0;
    virtual int validColorsCount (RotatingFigure* ) const {return 0;}
    virtual int getNCells() const =0;
    virtual void getCellsData(char* buf) const =0;
    virtual int notGrayColorsCount() const {return 0;}
#ifdef WIN32
    virtual bool isCube() const {return false;}
    virtual void getCubicAreas() {}
    virtual void clear() {};
#endif
    bool needsCellDraw;
    bool needsFullDraw;
protected:
    static float sign (float p1x, float p1y, float p2x, float p2y, float p3x, float p3y);
    MainWidget * mainWidget;
    QVector3D rotatePoint(const QVector3D &v) const;
    bool little;
    static bool PointInTriangle(float ptx, float pty, float v1x, float v1y, float v2x, float v2y, float v3x, float v3y);
};

#endif // ROTATINGFIGURE_H
