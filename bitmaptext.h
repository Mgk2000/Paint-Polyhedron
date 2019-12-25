#ifndef BITMAPTEXT_H
#define BITMAPTEXT_H
#include <map>
#include <QPoint>
#include <QVector4D>
#include "drawingobject.h"
class Letter
{
protected:
    Letter(int _x, int _y, int _w, int _h );
    virtual ~Letter();
    float width, height;
    void init();
    friend class BitmapText;
    QPointF * vertices;
    float x, y;
    inline float X() const {return x;}
    inline void setX(float _x){x=_x;}
    inline float Y() const {return y;}
    inline void setY(float _y){y=_y;}

};
class View;
class Texture;
class BitmapText : public DrawingObject
{
public:
    BitmapText();
//    void draw(float x, float y, float scale, const Point4D &color, const char *txt);
//    void drawCenter(float x, float y, float scale, const Point4D &color, const char *txt);
    void initGL(QOpenGLShaderProgram* prog) override;
    void fillData() override;
    void draw() override;
    float textWidth(const char* txt);
    struct VertexData
    {
        QVector3D position;
        QVector2D texCoord;
        QVector4D colormult;
    };

    void fillVertexData(VertexData* buf);
    void addLetterData(VertexData* buf, wchar_t ch, float left,
                       float w, const QVector4D & colorMult);
    void setText(const QString & _text);
    QString text;
    bool needsRedraw;
    int nElements() const {return text.length() * 6;}
private:
    std::map <wchar_t, Letter*> letters;
    int cellSize;

};

#endif // BITMAPTEXT_H
