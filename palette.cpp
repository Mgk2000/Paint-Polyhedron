#include "palette.h"
#include "cube.h"
#include "mainwidget.h"
Palette::Palette(MainWidget* mw) : DrawingObject() ,
    needsRedraw(false), selColor(0), dw(3), mainWidget(mw)
{
    extern  QVector4D gameColors[];
    for (int i =0; i<6; i++)
            buttons[i].colorInd = i+1;

}

Palette::~Palette()
{

}

void Palette::initGL(QOpenGLShaderProgram *prog)
{
    DrawingObject::initGL(prog,1);
}

void Palette::draw()
{
    if (needsRedraw)
        fillData();
    m_program->bind();
    buffers[0].bind();
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinates
    offset += sizeof(QVector3D);

    int texcoordLocation = m_program->attributeLocation("a_texcoord");
    m_program->enableAttributeArray(texcoordLocation);
    m_program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw palette
    glDrawArrays(GL_TRIANGLES, 0, nElements());


}

void Palette::fillData()
{
    VertexData* vertices = new VertexData[nElements()];
    fillVertexData(vertices);
    buffers[0].bind();
    buffers[0].allocate(vertices, nElements() * sizeof(VertexData));
    delete[] vertices;
    needsRedraw = false;

}

void Palette::addRectData(const QRectF &r, const QVector2D texCoords,  VertexData *buf)
{
    buf[0].texCoord = texCoords;
    buf[1].texCoord = texCoords+ QVector2D(0.3333f, 0);
    buf[2].texCoord = texCoords+ QVector2D(0.3333f, 0.3333f);
    buf[3].texCoord = buf[2].texCoord;
    buf[4].texCoord = texCoords+ QVector2D(0.0f, 0.3333f);
    buf[5].texCoord = buf[0].texCoord;

    buf[0].position = QVector3D(r.left(), -r.bottom(), 0);
    buf[1].position = QVector3D(r.right(), -r.bottom(), 0);
    buf[2].position = QVector3D(r.right(), -r.top(), 0);
    buf[3].position = buf[2].position;
    buf[4].position = QVector3D(r.left(), -r.top(), 0);
    buf[5].position = buf[0].position;

}

void Palette::fillVertexData(VertexData* buf)
{
    int bufind =0;
    addRectData(QRectF(-1.0, -1.0, 0.75, 2.0), mainWidget->colorSquareTexCoords[selColor],&(buf[bufind]));
    bufind +=6;
    for (int i =0; i< 2; i++)
        for (int j =0; j< 3; j++)
    {
       addRectData(QRectF(-0.2 + 0.4*j ,(i-1)*1.0,  0.4,1.0),
                   mainWidget->colorSquareTexCoords[i*3+j+1], &(buf[bufind]));
       bufind +=6;
    }
    needsRedraw = false;
}

void Palette::onResize(QRect &_viewport)
{
    this->setRect(_viewport.x(), _viewport.y(), _viewport.width(), _viewport.height());
    float bh = (height() - dw * 3) * 0.5;
    float bw = (width() - dw * 4) * 0.333;
    //if (bw1 < bh) bh = bw1;
    for (int i =0; i<2; i++)
        for (int j=0; j<3; j++)
        {
            buttons[i*3+j].setX(dw + x() + j* (bw+dw));
            buttons[i*3+j].setY(dw + y() + i* (bh+dw));
            buttons[i*3+j].setWidth(bw);
            buttons[i*3+j].setHeight(bh);
        }
}

bool Palette::mouseClick(int mx, int my)
{
    if (!this->contains(mx,my))
            return false;
    int j = (mx - x()) * 5 / width() - 2;
    if (j<0 || j> 2) return false;
    int i = (my-y()) *2 /height();
    if (i<0 || i>1) return false;
    selColor = i*3 + j +1;
    needsRedraw = true;
    return true;
}
