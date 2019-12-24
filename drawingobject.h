#ifndef DRAWINGOBJECT_H
#define DRAWINGOBJECT_H
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
class QOpenGLShaderProgram;

class DrawingObject : protected QOpenGLFunctions
{
public:
    DrawingObject();
    void initGL(QOpenGLShaderProgram* prog, int _nbuffers);
    virtual ~DrawingObject();
    virtual void fillData() = 0;
    virtual void draw() = 0;
protected:
    QOpenGLShaderProgram * m_program;
    QList <QOpenGLBuffer> buffers;
};

#endif // DRAWINGOBJECT_H
