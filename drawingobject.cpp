#include "drawingobject.h"

DrawingObject::DrawingObject()
{
}

void DrawingObject::initGL(QOpenGLShaderProgram *prog, int _nbuffers)
{
    m_program = prog;
    initializeOpenGLFunctions();
    for (int i=0; i< _nbuffers; i++)
    {
        QOpenGLBuffer buf;
        buffers.append(buf);
        buffers[i].create();
    }

}

DrawingObject::~DrawingObject()
{
    for (int i=0; i< buffers.count() ; i++)
        buffers[i].destroy();
    buffers.clear();
}
