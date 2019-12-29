#include "polyhedron.h"
#include "cube.h"
#include "mainwindow.h"

PolyhedronBase::PolyhedronBase(MainWidget * mw, bool _little) : RotatingFigure (mw, _little)
{

}

PolyhedronBase::~PolyhedronBase()
{
    delete[] vertexData;
}
void PolyhedronBase::init()
{
/*    for (int i=0; i< faces.length(); i++)
        faces[i].color = i % 6 +1;*/

    for (int i=0; i< faces.length(); i++)
        faces[i].fillVertexData(this, vertexData + i*3);
}
void PolyhedronBase::draw()
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
void PolyhedronBase::setData(const uchar *data)
{
    for (int i =0; i<faces.length(); i++)
        setFaceColor (i,data[i]);
}

void PolyhedronBase::fillData()
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

int PolyhedronBase::validColorsCount(RotatingFigure *lf) const
{
    return 0;
}

void PolyhedronBase::initGL(QOpenGLShaderProgram *prog)
{
    DrawingObject::initGL(prog,1);
}
void PolyhedronBase::createFacesFromVertices()
{
    for (int i =0; i< vertices.length()-2; i++)
        for (int j = i+1; j< vertices.length()-1; j++)
            for (int k = j+1; k< vertices.length(); k++)
                if (isTriangleFace(i, j ,k))
                {
                    bool cw = isClockWise(i,j,k);
                    if (cw)
                    {
                        _Face f (i,k, j);
                        faces.append(f);
                    }
                    else
                    {
                        _Face f (i,j,k);
                        faces.append(f);
                    }
                }
}

void PolyhedronBase::createEdgesFromFaces()
{
    edges.clear();
    for (int i=0; i< faces.length(); i++)
        for (int j=0; j<3; j++)
        {
            int iA = faces[i].vertices[j];
            int iB = faces[i].vertices[(j+1) %3];
            if (!edgeExists(iA, iB))
                  edges.append(Edge(iA, iB));
        }
}

bool PolyhedronBase::edgeExists(int iA, int iB)
{
    for (int i=0; i< edges.count(); i++)
        if (Edge(iA, iB) == edges[i])
            return true;
    return false;
}
void PolyhedronBase::setDivision(int div)
{
/*    int nf =0;
    for (int i =0; i< vertices.length()-2; i++)
        for (int j = i+1; j< vertices.length()-1; j++)
            for (int k = j+1; k< vertices.length(); k++)
                if (isTriangleFace(i, j ,k))
                    nf ++;
    qDebug() << "Faces=" << nf;*/
    float maxLen =10* radius;
    for (int i =0; i<  div; i++)
    {
        increaseDivision(maxLen * 1.9);
        maxLen =0;
        for (int j =0; j< edges.count(); j++)
        {
            float d = edgeLength(i);
            if (d > maxLen)
                maxLen =d;
        }
    }
}

void PolyhedronBase::increaseDivision(float maxLen)
{
/*    struct IVertex : public Vertex
    {
        int parents[2];
    };
    QList <IVertex> newPoints;
    for (int i =0; i< edges.length(); i++)
    {
        IVertex iv;
        iv.vertex = (vertices[edges[i].vertices[0]].vertex +
                vertices[edges[i].vertices[1]].vertex).normalized() * radius;
        iv.parents[0] = edges[i].vertices[0];
        iv.parents[1] = edges[i].vertices[1];
    }*/
    float r1 = vertices[0].vertex.length();
    for (int i =0; i< edges.length(); i++)
    {
        Vertex iv;
        iv.vertex = (vertices[edges[i].vertices[0]].vertex +
                vertices[edges[i].vertices[1]].vertex).normalized() * r1;
        vertices.append(iv);
     }
    for (int i=0; i < vertices.length(); i++)
        qDebug() << "vertices[" << i << "].length=" << vertices[i].vertex.length();

    faces.clear();
    edges.clear();
    int nf =0;
    for (int i =0; i< vertices.length()-2; i++)
        for (int j = i+1; j< vertices.length()-1; j++)
        {
            float d1 = (vertices[i].vertex -vertices[j].vertex).length();
            if ( d1 > maxLen)
                continue;
            for (int k = j+1; k< vertices.length(); k++)
            {
                float d2 = (vertices[k].vertex -vertices[j].vertex).length();
                if (d2 >maxLen)
                    continue;
                float d3 = (vertices[i].vertex -vertices[k].vertex).length();
                if (d3 >maxLen)
                    continue;
                if (isTriangleFace(i, j ,k))
                {
                    nf ++;
                    bool cw = isClockWise(i,j,k);
                    if (cw)
                    {
                        _Face f (i,k, j);
                        faces.append(f);
                    }
                    else
                    {
                        _Face f (i,j,k);
                        faces.append(f);
                    }
//                    faces[nf-1].color = (i + j + k) % 6 +1;
/*                    if (faces[i].color <1 || faces[i].color> 6)
                        faces[i].color = faces[i].color;*/

                    if (!isEdgeExists(i,j))
                            edges.append(Edge(i,j));
                    if (!isEdgeExists(i,k))
                            edges.append(Edge(i,k));
                    if (!isEdgeExists(j,k))
                            edges.append(Edge(j,k));
                }
            }
        }
}

void PolyhedronBase::getCellsData(char *buf) const
{
    for (int i = 0 ; i< faces.length(); i++)
        buf[i] = faces[i].color;
}

bool PolyhedronBase::isTriangleFace(int iA, int iB, int iC) const
{
    QVector3D AB =  vertices[iB].vertex - vertices[iA].vertex;
    QVector3D AC =  vertices[iC].vertex - vertices[iA].vertex;
    QVector3D n =QVector3D::crossProduct ( AB, AC);
    int firsti = 0;
    float p;
    for (int i =0; i< vertices.length(); i++)
        if (i!=iA  &&  i != iB && i != iC)
        {
            p = QVector3D::dotProduct(vertices[i].vertex - vertices[iA].vertex, n);
            if (qAbs(p) >0.0001)
            {
                firsti = i;
                break;
            }
        }
    for (int i = firsti+1; i< vertices.length(); i++)
        if (i!=iA  &&  i != iB && i != iC)
        {
            float p1 = QVector3D::dotProduct(vertices[i].vertex - vertices[iA].vertex, n);
            if (p1 * p < 0)
            {
                qDebug() << iA << iB << iC << "NOT Face";
                return false;
            }
        }

    qDebug() << iA << iB << iC << "Face";
    return true;
}

bool PolyhedronBase::isEdgeExists(int iA, int iB) const
{
    for (int i =0; i< edges.length(); i++)
        if ((edges[i].vertices[0] == iA && edges[i].vertices[1] == iB) ||
            (edges[i].vertices[1] == iA && edges[i].vertices[0] == iB))
            return true;
    return false;
}

bool PolyhedronBase::isClockWise(int i, int j, int k) const
{
    QVector3D v1 = vertices[i].vertex - vertices[j].vertex;
    QVector3D v2 = vertices[i].vertex - vertices[k].vertex;
    QVector3D n = QVector3D::crossProduct(v1, v2);
    float p = QVector3D::dotProduct(vertices[i].vertex, n);
    return p>0;
}
void PolyhedronBase::setNcells(int nc)
{

}

int PolyhedronBase::nElements() const
{
    return faces.count() * 3;
}

void PolyhedronBase::saveVertexInfo()
{
    int fsize = sizeof(int) * 2 +
            faces.length() * sizeof(int) * 3 +
            vertices.length() * sizeof(QVector3D);
    char* buf = new char[fsize];
    char * obuf = buf;
    int ii;
    float ff;
    //int* pnvert = (int*)buf;
    ii = vertices.length();
    memcpy(buf, &ii, 4);
   int nf = faces.length();
   memcpy(buf +4, &nf, 4);
   buf +=8;
   for (int i =0; i<vertices.length(); i++)
   {
       ff = vertices[i].vertex.x();
       memcpy(buf, &ff, sizeof(float));
       buf += sizeof(float);
       ff = vertices[i].vertex.y();
       memcpy(buf, &ff, sizeof(float));
       buf += sizeof(float);
       ff = vertices[i].vertex.z();
       memcpy(buf, &ff, sizeof(float));
       buf += sizeof(float);
   }
   for (int i=0; i< nf ; i++)
       for (int j=0; j<3; j++)
       {
           ii = faces[i].vertices[j];
           memcpy(buf, &ii, sizeof(ii));
           buf += sizeof(int);
       }
   QFile f (QString((mainWidget->projectDir() + "/settings/%1_%2.vert")
         .arg(mainWidget->gameStartInfo.type)
         .arg(mainWidget->gameStartInfo.division)));
   f.open(QIODevice::WriteOnly);
   f.write(obuf, fsize);
   delete[] obuf;

}

void PolyhedronBase::loadVertexInfo()
{
    char * buf = mainWidget->gameStartInfo.vertexInfo;
    int nv = *((int*) buf);
    int nf = *((int*) buf +1);
    QVector3D* pv = (QVector3D*)(buf+8);
    vertices.reserve(nv);
    for (int i=0; i< nv; i++)
    {
        Vertex v(pv[i].x(), pv[i].y(), pv[i].z() );
        vertices.append(v);
    }
    int* pf = (int*) (buf + 8 + nv* sizeof(QVector3D));
   faces.reserve(nf);
   for (int i=0; i< nf*3; i+=3)
   {
       _Face f(pf[i], pf[i+1], pf[i+2]);
        faces.append(f);
   }
}

void PolyhedronBase::setFaceColor(uint nf, int iColor)
{
    faces[nf].color = iColor;
    faces[nf].fillVertexData(this, vertexData + nf*3);

}
Vertex::Vertex(float _x, float _y, float _z)
{
    vertex = QVector3D(_x, _y, _z);
}

_Face::_Face(int indA, int indB, int indC) : color(0)
{
    vertices.append(indA);
    vertices.append(indB);
    vertices.append(indC);
}

const QVector3D &_Face::vertex(PolyhedronBase * parent, int ind) const
{
    return parent->vertices[vertices[ind]].vertex;
}

void _Face::fillVertexData(PolyhedronBase* parent , CubeVertexData *buf)
{

    buf[0].position = vertex(parent, 0);
    buf[1].position = vertex(parent, 1);
    buf[2].position = vertex(parent, 2);
    const QVector2D &texCoords = parent->mainWidget->colorSquareTexCoords[color];
    float dt = parent->little? 0.05f: 0;
    buf[0].texCoord = texCoords + QVector2D(dt*2,dt);
    buf[1].texCoord = texCoords+ QVector2D(0.3333f, 0) + QVector2D(-dt,dt);
    buf[2].texCoord = texCoords+ QVector2D(0.333f, 0.3333f)+ QVector2D(-dt,-dt *2);
    QVector3D n= (QVector3D::crossProduct ( vertex(parent, 1) - vertex(parent, 0), vertex(parent, 2) -
                                            vertex(parent, 1))).normalized();
    for (int i =0; i< 3; i++)
        buf[i].normal = n;
}

Edge::Edge(int indA, int indB)
{
    vertices[0] = indA;
    vertices[1] = indB;
}

bool Edge::isEqual(const Edge &e2)
{
    return (vertices[0] == e2.vertices[0] &&
            vertices[1] == e2.vertices[1]) ||
            (vertices[0] == e2.vertices[1] &&
            vertices[1] == e2.vertices[0]);
}

float PolyhedronBase::edgeLength(int i)
{
    int i0 = edges[i].vertices[0];
    int i1 = edges[i].vertices[1];
    return (vertices[i0].vertex - vertices[i1].vertex).length();
}


Polyhedron::Polyhedron(MainWidget *mw) : PolyhedronBase(mw, false)
{
}
int Polyhedron::pick(float mx, float my, int icolor)
{
    int ret =0;
    uint nv = (uint) vertices.length();
    QVector3D* rotatedVertices = new QVector3D [nv];
    bool* pointExists = new bool[nv];
    for (uint i =0; i< nv; i++)
        pointExists[i] = false;
    QVector3D v[3];
    QList<uint> pickFaces;
    uint nf = faces.length();
    for (uint i =0; i< nf; i++)
    {
        _Face& f = faces[i];
        for (uint j =0; j< 3; j++)
        {
            uint vind = f.vertices[j];
            if (pointExists[vind])
                v[j] = rotatedVertices[vind];
            else
            {
                v[j] = rotatePoint(vertices[vind].vertex);
                rotatedVertices[vind] = v[j];
                pointExists[vind] = true;
            }
         }
         if (PointInTriangle(mx, my, v[0].x(), v[0].y(), v[1].x(), v[1].y(), v[2].x(), v[2].y()) )
         {
             pickFaces.append(i);
             if (pickFaces.length() == 2)
             {
                 float facez[2];
                 for (uint k =0; k<2; k++)
                 {
                     uint indf = pickFaces[k];
                     facez[k] = 0;
                     _Face& f = faces[indf];
                     for (uint j =0; j< 3; j++)
                     {
                         float z = rotatedVertices[f.vertices[j]].z();
                         facez[k] = facez[k] + z;
                     }
                 }
                 int nearInd = facez[0] < facez[1] ? 0 : 1;
#ifdef WIN32
                 int indf = pickFaces[nearInd];
                 for(int ii = 0; ii<3; ii++)
                     qDebug() << vertices[faces[indf].vertices[ii]].vertex;
#endif
//                 int farInd = 1- nearInd;
                 ret = pick (pickFaces[nearInd], icolor);
                     goto ret;
//                 qDebug() << "Near face =" << pickFaces[nearInd] << "Color=" << faces[pickFaces[nearInd]].color;
//                 qDebug() << "Far face =" << pickFaces[farInd] << "Color=" << faces[pickFaces[farInd]].color;
             }
         }
    }
ret:
    delete[] rotatedVertices;
    delete[] pointExists;
    return ret;
}

int Polyhedron::pick(int nf, int iColor)
{
    int oldColor = faces[nf].color;
    int trueColor = littlePoly->faces[nf].color;
    int ret =0;
    if (iColor != trueColor)
    {
        mainWidget->sound (0);
        if (oldColor == trueColor)
            ret = -1;
        else ret =0;
    }
    else
    {
        if (oldColor == trueColor)
            ret = 0;
        else
        {ret =1;
        mainWidget->sound(1);
        }
    }
    setFaceColor(nf, iColor);
    saveMove(nf,  iColor);
    selIndex = nf  * 3;
    if (iColor != oldColor)
      needsCellDraw = true;
    return ret;
}
void Polyhedron::saveMove(int iface, uchar colorInd)
{
#ifdef WIN32
    if (mainWidget->gameStartInfo.editor)
        return;
#endif
     mainWidget->gameStartInfo.currGameFile->seek(iface);
     char color = colorInd;
     mainWidget->gameStartInfo.currGameFile->write(&color,1);
}
int Polyhedron::validColorsCount(RotatingFigure *lf) const
{
    LittlePolyhedron* lp = (LittlePolyhedron*) lf;
    int nc = 0;
    for (int i=0; i < faces.length(); i++)
        if (faces[i].color == lp->faces[i].color)
            nc++;
    return nc;
}

int Polyhedron::notGrayColorsCount() const
{
    int nc =0;
    for (int i=0; i< faces.length(); i++)
        if (faces[i].color)
            nc++;
    return nc;
}
#ifdef WIN32
void Polyhedron::getCubicAreas()
{
    getPoles();
}

void Polyhedron::getPoles()
{
    int* vertFaces = new int[vertices.count()];
    poles.clear();
    polarFaces.clear();
    int vc = vertices.count();
    for(int i = 0; i < vc; i++)
        vertFaces[i] = 0;
    for (int i =0; i< faces.count(); i++)
        for (int j = 0; j < faces[i].vertices.count(); j++)
            vertFaces[faces[i].vertices[j]] ++;
    for(int i = 0; i < vc; i++)
        if (vertFaces[i] ==4)
        {
            poles.append(i);
        }
    for (int i=0; i<poles.count(); i++)
    {
        int ind = poles[i];
        bool found =0;
        for (int j =0; j< faces.count(); j++)
            for (int k = 0; k<faces[j].vertices.count(); k++)
                if(faces[j].vertices[k] == ind)
                {
                    if (!found)
                    {
                        found = true;
                        QList<int> l;
                        polarFaces.append(l);
                    }
                    polarFaces[polarFaces.length() -1].append(j);
                }
    }
    for (int i=0; i< poles.count(); i++)
    {
        qDebug() << "Pole[" << i << "]=" << vertices[poles[i]].vertex;
        qDebug() << "polarFaces=" << polarFaces[i].count();
        for (int j =0; j< polarFaces[i].count(); j++)
            setFaceColor(polarFaces[i][j], i+1);
    }
    needsFullDraw = true;
    delete[] vertFaces;
}
#endif
LittlePolyhedron::LittlePolyhedron(Polyhedron *bigPoly): PolyhedronBase(nullptr, true)
{
    vertices= bigPoly->vertices;
    edges = bigPoly->edges;
    int nf = bigPoly->faces.length();
    faces.reserve(nf);
    for (int i =0; i< nf; i++)
        faces.append(_Face(bigPoly->faces[i]));
    mainWidget = bigPoly->mainWidget;
    bigPoly->littlePoly = this;
}

