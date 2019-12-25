#include "buttons.h"

Buttons::Buttons() : needsRedraw(true)
{
    state[0] = state[1] = true;
}

void Buttons::initGL(QOpenGLShaderProgram *prog)
{
    DrawingObject::initGL(prog,1);
}

void Buttons::fillData()
{
    VertexData* vertices = new VertexData[nElements()];
    fillVertexData(vertices);
    buffers[0].bind();
    buffers[0].allocate(vertices, nElements() * sizeof(VertexData));
    needsRedraw = false;
    delete[] vertices;
}

void Buttons::draw()
{
    if (needsRedraw)
        fillData();
    buffers[0].bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for color
    offset += sizeof(QVector3D);

    int texcoordLocation = m_program->attributeLocation("a_texcoord");
    m_program->enableAttributeArray(texcoordLocation);
    m_program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Offset for color multiplicator
    offset += sizeof(QVector2D);
    int colorMultLocation = m_program->attributeLocation("a_colormult");
    m_program->enableAttributeArray(colorMultLocation);
    m_program->setAttributeBuffer(colorMultLocation, GL_FLOAT, offset, 4, sizeof(VertexData));

    glDrawArrays(GL_TRIANGLES, 0, nElements());
}

void Buttons::fillVertexData(VertexData *buf)
{
    for (int i=0; i<2; i++)
        fillButtonVertexData(buf + i*6, i);
}

void Buttons::fillButtonVertexData(VertexData *buf, int ib)
{
    for (int i=0; i<6; i++)
        buf[i].colormult = state[ib] ? QVector4D(0,1,0,1) : QVector4D(0.5, 0.5, 0.5, 1);
    float x = -1 + ib * 1.0f;
    buf[0].position = QVector3D(x,-1,0);
    buf[1].position = QVector3D(x+1,-1,0);
    buf[2].position = QVector3D(x+1, 1,0);
    buf[3].position = buf[2].position;
    buf[4].position = QVector3D(x, 1,0);
    buf[5].position = buf[0].position;
    x = ib * 0.5;
    buf[0].texCoord = QVector2D(x, 0.5);
    buf[1].texCoord = QVector2D(x+0.5, 0.5);
    buf[2].texCoord = QVector2D(x+0.5, 1);
    buf[3].texCoord = buf[2].texCoord;
    buf[4].texCoord = QVector2D(x, 1);
    buf[5].texCoord = buf[0].texCoord;
}
void Buttons::onResize(QRect &_viewport)
{
    this->setRect(_viewport.x(), _viewport.y(), _viewport.width(), _viewport.height());
}

bool Buttons::mouseClick(int mx, int my)
{
    if (!this->contains(mx,my))
            return false;
    int j = (mx - x()) *2 /width();
    state[j]  = ! state[j];
    if (j==1)
        emit musicButtonPressed();
    needsRedraw = true;
    return true;
}
