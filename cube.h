#ifndef CUBE_H
#define CUBE_H
#include <QVector4D>
#include <QVector3D>
#include <QVector>
#include "mainwidget.h"
#include "rotatingfigure.h"

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

class Cube : public RotatingFigure
{
public:
    Cube(MainWidget * mw, bool _little);
    virtual ~Cube();
    void fillData() override;
    void draw() override;
    void initGL(QOpenGLShaderProgram* prog) override;
    void init() override;
    int ncells;
    CubeVertexData* vertices;
    int selIndex;
    Face faces[6];
    void setNcells(int nc);
    void setCellColor(short face, short r, short c, short colorInd);
    void fillVertexData(CubeVertexData* buf);
    void getCorners();
    int nElements() const;
    int pick(float mx, float my, int icolor) override;

    int pick(int nf, float x, float y, int icolor);
    void rowColFromXY (int nf, float x, float y, int* r, int* c);
    void setData(const uchar* data) override;
    void duplicateFace (int nf);
    void rotateFace (int nf);
    void fillFace(int nf, int iColor);
    int validColorsCount (RotatingFigure* lf) override;
protected:
private:
    QVector3D faceCorners[6][4];
    void saveMove(int face, int row, int col, uchar colorInd);

  friend struct Face;
  friend struct Cell;
};

#endif // CUBE_H
