#include "polyhedron.h"
#include "cube.h"

Polyhedron::Polyhedron(MainWidget * mw, bool _little) : RotatingFigure (mw, _little)
{

}
void Polyhedron::init()
{
    for (int i=0; i< faces.length(); i++)
        faces[i].fillVertexData(vertexData + i*3);
}
void Polyhedron::draw()
{
    if (needsCellDraw || needsFullDraw)
        fillData();
    buffers[0].bind();
    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(CubeVertexData));

    // Offset for normals
    offset += sizeof(QVector3D);

    int normalLocation = m_program->attributeLocation("a_normal");
    m_program->enableAttributeArray(normalLocation);
    m_program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(CubeVertexData));
    // Offset for texture coordinates
    offset += sizeof(QVector3D);

    int texcoordLocation = m_program->attributeLocation("a_texcoord");
    m_program->enableAttributeArray(texcoordLocation);
    m_program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(CubeVertexData));

    glDrawArrays(GL_TRIANGLES, 0, nElements());


}
void Polyhedron::setData(const uchar *data)
{

}
int Polyhedron::pick(float mx, float my, int icolor)
{
    return 0;
}

void Polyhedron::fillData()
{
    buffers[0].bind();
    if (needsFullDraw)
    {
        buffers[0].allocate(vertexData, nElements() * sizeof(CubeVertexData));
        needsFullDraw = false;
    }
    else if (needsCellDraw)
        buffers[0].write(selIndex * sizeof(CubeVertexData),
                &vertexData[selIndex],
                3 * sizeof(CubeVertexData));
    needsCellDraw = false;

}

int Polyhedron::validColorsCount(RotatingFigure *lf)
{
    return 0;
}

void Polyhedron::initGL(QOpenGLShaderProgram *prog)
{
    DrawingObject::initGL(prog,1);
}
void Polyhedron::setNcells(int nc)
{

}

int Polyhedron::nElements() const
{
    return faces.count() * 3;
}
Vertex::Vertex(float _x, float _y, float _z)
{
    vertex = QVector3D(_x, _y, _z);
}

_Face::_Face(Polyhedron* _parent, int indA, int indB, int indC) : parent(_parent)
{
    vertices.append(indA);
    vertices.append(indB);
    vertices.append(indC);
}

const QVector3D &_Face::vertex(int ind) const
{
    return parent->vertices[vertices[ind]].vertex;
}

void _Face::fillVertexData(CubeVertexData *buf)
{

    buf[0].position = vertex(0);
    buf[1].position = vertex(1);
    buf[2].position = vertex(2);
    const QVector2D &texCoords = parent->mainWidget->colorSquareTexCoords[color];
    buf[0].texCoord = texCoords;
    buf[1].texCoord = texCoords+ QVector2D(0.3333f, 0);
    buf[2].texCoord = texCoords+ QVector2D(0.3333f, 0.3333f);
    QVector3D n= (QVector3D::crossProduct ( vertex(1) - vertex(0), vertex(2) - vertex(1))).normalized();
    for (int i =0; i< 3; i++)
        buf[i].normal = n;
}

Edge::Edge(int indA, int indB)
{
    vertices[0] = indA;
    vertices[1] = indB;
}
