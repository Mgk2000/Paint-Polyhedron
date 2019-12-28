#include "cube.h"
#include "geometry.h"
#include <QtMath>
QVector3D faceCorners[] =
{  QVector3D(-1.0,-1.0,-1.0),  // front left bottom  0
   QVector3D(1.0,-1.0,-1.0), // front right bottom   1
   QVector3D (1.0,1.0,-1.0), //front right top       2
   QVector3D(-1.0,1.0,-1.0),  //front left top       3

   QVector3D(-1.0,-1.0,1.0), //back left bottom      4
   QVector3D(1.0,-1.0, 1.0), //back right bottom     5
   QVector3D (1.0,1.0, 1.0), //back right top        6
   QVector3D(-1.0,1.0,1.0)}; //back left top         7

Cube::Cube(MainWidget * mw, bool _little) : RotatingFigure(mw, _little), vertexData(nullptr)
{
    setNcells(1);
    faces[0].init(0, 2,2,0); //front
    faces[1].init(5, -2,2,0); //back
    faces[2].init(0, 0, 2, 2); //left
    faces[3].init(5, 0, 2, -2); //right
    faces[4].init(4,2 ,0 ,-2);  // bottom
    faces[5].init(3,2,0 ,2); //top
    if (_little) //i.e littleCube - draw it faces
    {
        setCellColor(0,0,0,1);
        setCellColor(1,0,0,2);
        setCellColor(2,0,0,3);
        setCellColor(3,0,0,4);
        setCellColor(4,0,0,5);
        setCellColor(5,0,0,6);
    }
    else
        getCorners();
}

Cube::~Cube()
{

}

void Cube::fillData()
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
                6 * sizeof(CubeVertexData));
    needsCellDraw = false;
}

void Cube::draw()
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

void Cube::initGL(QOpenGLShaderProgram *prog)
{
    DrawingObject::initGL(prog,1);
}

void Cube::init()
{
    setNcells(mainWidget->gameStartInfo.ncells);
}

void Cube::setNcells(int nc)
{
    ncells = nc;
    for (int i=0; i<6; i++)
    {
        faces[i].cube = this;
        faces[i].setNcells(nc);
    }
    if (vertexData)
        delete[] vertexData;
    vertexData = new CubeVertexData [6 * 6 *ncells * ncells];
    fillVertexData(vertexData);
}

void Cube::setCellColor(short nface, short r, short c, short colorInd)
{
    faces[nface].setCellColor(r,c,colorInd);
    faces[nface].addCellVertexData(&vertexData[ 6* (nface * ncells * ncells + ncells * r + c)],
            mainWidget->colorSquareTexCoords[colorInd], r , c);
}

void Cube::fillVertexData(CubeVertexData *buf)
{
    for (int i = 0; i<6; i++)
        faces[i].fillVertexData(buf + i*ncells*ncells*6, mainWidget->colorSquareTexCoords);
}

void Cube::getCorners()
{
    for (int i=0; i< 6; i++)
        faces[i].getCorners(&faceCorners[i][0],&faceCorners[i][1],
                &faceCorners[i][2],&faceCorners[i][3]);
}

int Cube::nElements() const
{
    return ncells* ncells * 6 * 6;

}
int Cube::pick(float mx, float my, int icolor)
{
    QVector3D corners[4], saveCorners[4];
    bool saved = false;
    float saveZ;
    bool pinside = false;
    QVector3D res1, saveRes1;
    int saveFace;
    for (int i =0; i< 6; i++)
    {
        corners[0] = rotatePoint(faceCorners[i][0]);
        corners[1] = rotatePoint(faceCorners[i][1]);
        corners[2] = rotatePoint(faceCorners[i][2]);
        corners[3] = rotatePoint(faceCorners[i][3]);
        QVector3D mv (mx, my, 0);
        pointInParallelogram(corners[0], corners[1],corners[2], corners[3],mv, res1, &pinside);
        if (pinside)
        {
            if (!saved)
            {
                saveZ = 0;
                for (int j=0; j<4; j++)
                {
                   saveCorners[j] = corners[j];
                   saveZ = saveZ + corners[j].z();
                }
                saveRes1 = res1;
                saveFace = i;
                saved = true;
            }
            else
            {
                float newZ = 0;
                for (int j=0; j<4; j++)
                    newZ = newZ + corners[j].z();
                    if (newZ < saveZ)
                    {
//                        qDebug() << "mv=" << mv.x() << mv.y() <<
//                            "face" << i << "< saveFace" << saveFace << " corner=" <<
//                                corners[2].x() << corners[2].y();
//                            qDebug() << "cubeFace XY" << res1.x() << res1.y()<<
//                                        qFloor(res1.x() * cube->ncells) << qFloor(res1.y()* cube->ncells);
                         int p =pick(i, res1.x(), res1.y(), icolor);
                         needsCellDraw = true;
                         return p;
                     }
                     else /*if (corners[j].z() > saveCorners[k].z() +0.1)*/
                     {
//                            qDebug() << "mv=" << mv.x() << mv.y() <<
//                            "saveFace" << saveFace << "<face" << i << " corner=" <<
//                                saveCorners[2].x() << saveCorners[2].y();
//                            qDebug() << "cubeFace XY" << saveRes1.x() << saveRes1.y() <<
//                                        qFloor(saveRes1.x() * cube->ncells) << qFloor(saveRes1.y()* cube->ncells);
                            int p = pick(saveFace, saveRes1.x(), saveRes1.y(), icolor);
                            needsCellDraw = true;
                            return p;
                      }
            }
        }
    }
    return 0;

}

int Cube::pick(int nf, float x, float y, int iColor)
{
//    qDebug() << "Face=" << nf;
    if (mainWidget->duplicatePending)
    {
        duplicateFace(nf);
        mainWidget->duplicatePending = false;
        needsFullDraw = true;
        mainWidget->update();
        return 0;
    }
    else if (mainWidget->rotatePending)
    {
        rotateFace(nf);
        mainWidget->rotatePending = false;
        needsFullDraw = true;
        mainWidget->update();
        return 0;
    }
    else if (mainWidget->fillFacePending)
    {
        fillFace(nf, iColor);
        mainWidget->fillFacePending = false;
        needsFullDraw = true;
        mainWidget->update();
        return 0;
    }
    int row, col;
    rowColFromXY(nf,x,y, & row, &col);
   // qDebug() << "pick row,col = " << row <<col;
    int oldColor = faces[nf].cells[row][col].colorInd;
    int trueColor = ((Cube*)mainWidget->littleFigure)->faces[nf].cells[row][col].colorInd;
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
    setCellColor(nf, row,col, iColor);
    saveMove(nf, row,col, iColor);
    selIndex = (nf * ncells  * ncells + row * ncells +col) * 6;
    if (iColor != oldColor)
      needsCellDraw = true;
    return ret;
}

void Cube::rowColFromXY(int nf, float x, float y, int *r, int *c)
{
    int i = qFloor (x * ncells);
    int j = qFloor (y * ncells);
    if (nf <4)
    {
        *r = j;
        *c = i;
    }
    else
    {
        *r = i;
        *c = j;
    }

}

void Cube::setData(const uchar *data)
{
    for (int i =0; i<6; i++)
            for (int j =0; j< ncells; j++)
                for (int k =0; k< ncells; k++)
                {
                    int ind = i * ncells * ncells + j* ncells + k;
                    faces[i].setCellColor(j,k,data[ind]);
                }
    fillVertexData(vertexData);
}

void Cube::duplicateFace(int nf)
{
    for (int i =0; i< 6 ; i++)
        if (i != nf)
            faces[i].duplicate(faces[nf], i);
    fillVertexData(vertexData);

}

void Cube::rotateFace(int nf)
{
    faces[nf].rotate();
    fillVertexData(vertexData);
}

void Cube::fillFace(int nf, int iColor)
{
    faces[nf].fillColor(iColor);
    fillVertexData(vertexData);
}

int Cube::validColorsCount(RotatingFigure *lf) const
{
    Cube* littleCube = (Cube*) lf;
    int vc =0;
    for (int i=0; i< 6; i ++)
        for (int j =0; j< ncells; j++)
            for (int k =0; k< ncells; k++)
                if (faces[i].cells[j][k].colorInd == littleCube->faces[i].cells[j][k].colorInd)
                    vc ++;
    return vc;

}

void Cube::getCellsData(char *buf) const
{
    for (int i =0; i <6; i++)
        for (int j=0; j< ncells; j++)
            for (int k=0; k < ncells; k++)
            {
                int ind = i*ncells * ncells + j* ncells + k;
                char colorInd = faces[i].cells[j][k].colorInd;
                buf[ind] = colorInd;
            }
}

void Cube::saveMove(int face, int row, int col, uchar colorInd)
{
    if (mainWidget->gameStartInfo.editor)
        return;
     int ind = face * ncells * ncells + row  * ncells + col;
     mainWidget->gameStartInfo.currGameFile->seek(ind);
     char color = colorInd;
     mainWidget->gameStartInfo.currGameFile->write(&color,1);
}

Cell::Cell()
{
    colorInd = 0;
}

void Cell::setColor(short _ci)
{
    colorInd = _ci;
}

Face::~Face()
{
    cells.clear();

}

void Face::setNcells(int nc)
{
    ncells = nc;
    cells.clear();
    for (int i=0; i< nc; i++)
    {
        QVector<Cell> stride;
             for (int j=0; j< nc; j++)
             {
                 Cell c;
                 stride.append(c);
             }
        cells.append(stride);
    }
}

void Face::setCellColor(short r, short c, short colorInd)
{
    cells[r][c].setColor(colorInd);
}

void Face::fillVertexData(CubeVertexData *buf, QVector2D  texCoords[])
{
    for (int i=0; i< ncells; i++)
        for (int j = 0; j<ncells; j++)
            addCellVertexData(&buf[(i*ncells+ j) * 6],texCoords[cells[i][j].colorInd], i, j);
}

void Face::addCellVertexData(CubeVertexData * buf, QVector2D& texCoords, int r, int c)
{
    float dt = 0.05;
    if (!cube->little)
    {
        buf[0].texCoord = texCoords + QVector2D (dt,0);
        buf[1].texCoord = texCoords+ QVector2D(0.3333f, 0);
        buf[2].texCoord = texCoords+ QVector2D(0.3333f, 0.3333f -dt);
        buf[3].texCoord = texCoords+ QVector2D(0.3333f-dt, 0.3333f );
        buf[4].texCoord = texCoords+ QVector2D(0.0f, 0.3333f);
        buf[5].texCoord = texCoords + QVector2D (0, dt);
    }
    else
    {
        buf[0].texCoord = texCoords + QVector2D (dt *2, dt);;
        buf[1].texCoord = texCoords+ QVector2D(0.3333f -dt, dt);;
        buf[2].texCoord = texCoords+ QVector2D(0.3333f - dt, 0.3333f -dt*2);;
        buf[3].texCoord = texCoords+ QVector2D(0.3333f-dt*2, 0.3333f-dt );
        buf[4].texCoord = texCoords+ QVector2D(dt, 0.3333f-dt);
        buf[5].texCoord = texCoords + QVector2D (dt, dt*2);
    }
    QVector3D n;
    if (incrZ == 0)
    {
        buf[0].position =  QVector3D(corner.x() + 1.0f * c *incrX/ncells,
                  corner.y() + 1.0f * r * incrY/ncells, corner.z());
        buf[1].position =  QVector3D(corner.x() + 1.0f * (c+1) *incrX/ncells,
                  corner.y() + 1.0f * r * incrY/ncells, corner.z());
        buf[2].position =  QVector3D(corner.x() + 1.0f * (c+1) *incrX/ncells,
                   corner.y() + 1.0f * (r+1) * incrY/ncells, corner.z());
        buf[3].position = buf[2].position;

        buf[4].position =  QVector3D(corner.x() + 1.0f * c *incrX/ncells,
                   corner.y() + 1.0f *(r+1) * incrY/ncells, corner.z());
        buf[5].position = buf[0].position;
        n = QVector3D (0, 0, (incrX * incrY <0) ?  -1.0 : 1.0);
    }
    else if (incrY ==0)
    {
        buf[0].position =  QVector3D(corner.x() + 1.0f * c *incrX/ncells, corner.y(),
                  corner.z() + 1.0f * r * incrZ/ncells);
        buf[1].position =  QVector3D(corner.x() + 1.0f * (c+1) *incrX/ncells, corner.y(),
                  corner.z() + 1.0f * r * incrZ/ncells);
        buf[2].position =  QVector3D(corner.x() + 1.0f * (c+1) *incrX/ncells, corner.y(),
                  corner.z() + 1.0f * (r+1) * incrZ/ncells);
        buf[3].position = buf[2].position;
        buf[4].position =  QVector3D(corner.x() + 1.0f * (c) *incrX/ncells,corner.y(),
                  corner.z() + 1.0f * (r+1) * incrZ/ncells);
        buf[5].position = buf[0].position;
        n = QVector3D (0, (incrX * incrZ >0) ?  -1.0 : 1.0, 0);
    }
    else
    {
        buf[0].position =  QVector3D(corner.x(), corner.y()  + 1.0f * c *incrY/ncells,
                  corner.z() + 1.0f * r * incrZ/ncells);
        buf[1].position =  QVector3D(corner.x(), corner.y()  + 1.0f * (c+1) *incrY/ncells,
                  corner.z() + 1.0f * r * incrZ/ncells);
        buf[2].position =  QVector3D(corner.x(), corner.y()  + 1.0f * (c+1) *incrY/ncells,
                  corner.z() + 1.0f * (r+1) * incrZ/ncells);
        buf[3].position = buf[2].position;
        buf[4].position =  QVector3D(corner.x(), corner.y()  + 1.0f * c *incrY/ncells,
                  corner.z() + 1.0f * (r+1) * incrZ/ncells);
        buf[5].position = buf[0].position;
        n = QVector3D ((incrZ * incrY < 0) ?  -1.0 : 1.0, 0, 0);

    }
    for (int i=0; i<6; i++)
        buf[i].normal = n;
}

void Face::init(int _corner, int _incrX, int _incrY, int _incrZ)
{
    corner = faceCorners[_corner];
    incrX = _incrX;
    incrY = _incrY;
    incrZ = _incrZ;
}

void Face::getCorners(QVector3D *A, QVector3D *B, QVector3D *C, QVector3D *D) const
{
    *A = corner;
    if (incrX == 0)
    {
        *B = QVector3D(corner.x(), corner.y() + incrY, corner.z());
        *C = QVector3D(corner.x(), corner.y() + incrY, corner.z()+ incrZ);
        *D = QVector3D(corner.x(), corner.y() , corner.z()+ incrZ);
    }
    else if (incrY ==0)
    {
        *B = QVector3D(corner.x(), corner.y() , corner.z() + incrZ);
        *C = QVector3D(corner.x() + incrX, corner.y(), corner.z() + incrZ);
        *D = QVector3D(corner.x() + incrX, corner.y(), corner.z());
    }
    else
    {
        *B = QVector3D(corner.x()+ incrX, corner.y() , corner.z());
        *C = QVector3D(corner.x()+ incrX, corner.y() + incrY , corner.z());
        *D = QVector3D(corner.x() , corner.y() +incrY , corner.z());
    }
}

void Face::duplicate(const Face &from, int colorShift)
{
    for (int i = 0; i< ncells; i++)
        for (int j=0; j<ncells; j++)
        {
            short c = from.cells[i][j].colorInd;
            short cc;
            if (!c)
                cc=0;
            else
                cc =((c-1) +colorShift) % 6 +1;
            setCellColor(i,j,cc);
        }
}

void Face::rotate()
{
    QVector<QVector<Cell>> c = cells;
    for (int i=0; i< ncells; i++)
        for (int j=0; j< ncells; j++)
            cells[i][j] = c[j][ncells-i-1];

}

void Face::fillColor(int cind)
{
    for (int i =0; i<ncells; i++)
        for (int j=0; j< ncells; j++)
            setCellColor(i,j,cind);
}
