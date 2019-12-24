#ifndef CUBE_H
#define CUBE_H
#include <QVector4D>
#include <QVector3D>
#include <QVector>
#include "mainwidget.h"
#include "drawingobject.h"

struct Cell
{
    Cell();
    void setColor(short _colorInd);
    short colorInd;

};
struct CubeVertexData
{
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
};
struct Face
{
    int ncells;
    ~Face();
    //int vertInd[4];
    void setNcells(int nc);
    QVector<QVector<Cell>> cells;
    void setCellColor(short r, short c, short colorInd);
    void fillVertexData(CubeVertexData* buf, QVector2D  texCoords[]);
    void addCellVertexData(CubeVertexData * buf,QVector2D& texCoords,int r, int c);
    int incrX, incrY, incrZ;
    QVector3D corner;
    void init(int _nCorner, int _incrX, int _incrY, int _incrZ);
    void getCorners(QVector3D *A,QVector3D *B,QVector3D *C,QVector3D *D) const;
    void duplicate(const Face& from, int colorShift);
    void rotate();
    void fillColor(int cind);
    Cube * cube;
};

class Cube : public DrawingObject
{
public:
    Cube(MainWidget * mw, bool _init);
    virtual ~Cube();
    void fillData() override;
    void draw() override;
    void intGL(QOpenGLShaderProgram* prog);
    int ncells;
    QVector2D texCoords[7];
    CubeVertexData* vertices;
    int selIndex;
    Face faces[6];
    void setNcells(int nc);
    void setCellColor(short face, short r, short c, short colorInd);
    void fillVertexData(CubeVertexData* buf);
    void getCorners(QVector3D* _corners);
    int nElements() const;
    int pick(int nf, float x, float y, int icolor);
    void rowColFromXY (int nf, float x, float y, int* r, int* c);
    bool needsCellDraw;
    void setData(const uchar* data);
    bool little;
    void duplicateFace (int nf);
    void rotateFace (int nf);
    void fillFace(int nf, int iColor);
    MainWidget* mainWidget;
    bool needsFullDraw;
protected:
  friend class Face;
  friend class Cell;
};

#endif // CUBE_H
