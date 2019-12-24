#ifndef BUTTONS_H
#define BUTTONS_H
#include <QPoint>
#include "mainwidget.h"

class Buttons :public QObject, public QRectF, public DrawingObject
{
    Q_OBJECT
public:
    Buttons();
    bool state[2];
    void intGL(QOpenGLShaderProgram* prog);

    void fillData() override;
    void draw() override;
    struct VertexData
    {
        QVector3D position;
        QVector2D texCoord;
        QVector4D colormult;
    };

    void fillVertexData(VertexData* buf);
    void fillButtonVertexData(VertexData* buf, int ib);
    void setSounds(bool b) {needsRedraw = true; state[0] = b;}
    void setMusic(bool b) {needsRedraw = true; state[1] = b;}
    bool soundPressed() const {return state[0];}
    bool musicPressed() const {return state[1];}
    int nElements() const {return 12;}
    bool mouseClick(int mx, int my);

    bool needsRedraw;

    void onResize(QRect &_viewport);
signals:
    void musicButtonPressed();
protected:

};

#endif // BUTTONS_H
