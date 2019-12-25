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
