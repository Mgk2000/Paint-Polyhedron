#ifndef PALETTE_H
#define PALETTE_H
#include <QRect>
#include <QVector4D>
#include "drawingobject.h"
struct VertexData;
class QOpenGLTexture;
class Cube;
class Button : public QRectF
{
public:
    int colorInd;

};
class Palette : public DrawingObject,  public QRectF
{
public:
    Palette();
    virtual ~Palette() override;
    void intGL(QOpenGLShaderProgram* prog);
    void draw() override;
    void fillData() override;
    bool needsRedraw;
    int nElements() const {return 42;}
    struct VertexData
    {
        QVector3D position;
        QVector2D texCoord;
    };

    void addRectData(const QRectF& r,const QVector2D texCoords,  VertexData* buf);
    void fillVertexData(VertexData* buf);
    void buttonVertexData();
    Button buttons[6];
    void onResize(QRect& _viewport);
    int selColor;
    bool mouseClick(int mx, int my);
    int dw;
    Cube * cube;
private:
    QOpenGLTexture * texture;

};

#endif // PALETTE_H
