#ifndef POLYHEDRON_H
#define POLYHEDRON_H
#include "rotatingfigure.h"
class PolyhedronBase;
struct Vertex
{
    Vertex() {}
    Vertex(float _x, float _y, float _z);
    QVector3D vertex;
};
#ifdef WIN32
struct Edge
{
    Edge (int indA, int indB);
    int vertices[2];
};
#endif
struct CubeVertexData;
struct _Face
{
    _Face(int indA, int indB, int indC);
    const QVector3D& vertex(PolyhedronBase* parent, int ind) const;
    void fillVertexData(PolyhedronBase* parent, CubeVertexData* buf);
    QList <uint> vertices;
    ushort color;
};
#ifdef WIN32
struct CubicArea
{
    int poles[3];
    QList<int> faces;
};
#endif
class PolyhedronBase : public RotatingFigure
{
public:
    PolyhedronBase(MainWidget * mw, bool _little);
    virtual ~PolyhedronBase();
    void init() override;
    void draw() override;
    void setData(const uchar *data) override;
    void fillData() override;
    int validColorsCount (RotatingFigure* lf) const override;
    void initGL(QOpenGLShaderProgram* prog) override;
    void setDivision(int div);
    void increaseDivision(float maxLen);
    int getNCells() const override {return faces.length();}
    void getCellsData(char* buf) const override;
protected:
    void subdivide();
    bool isTriangleFace(int iA, int iB, int iC) const;
    bool isEdgeExists(int IA, int iB) const;
    bool isClockWise(int i, int j, int) const;
    float inline edgeLength(int i);
    void setNcells(int nc);
    QList <Vertex> vertices;
#ifdef WIN32
    QList <Edge> edges;
    void clear() override;
#endif
    QList <_Face> faces;
    int nElements() const;
    CubeVertexData* vertexData;
    float radius;
    friend struct _Face;
    int selIndex;
#ifdef WIN32
    void saveVertexInfo();
#endif
    void loadVertexInfo();
    void setFaceColor(uint nf, int iColor);
#ifdef WIN32
    void createFacesFromVertices();
    void createEdgesFromFaces();
    void divide3();
#endif
};
class LittlePolyhedron;
class Polyhedron: public PolyhedronBase
{
public :
    Polyhedron(MainWidget* mw);
    int pick(float mx, float my, int icolor) override;
    int pick(int nf, int icolor);

    int validColorsCount(RotatingFigure *lf) const override;
    int notGrayColorsCount() const override;
protected:
#ifdef WIN32
    CubicArea cubicAreas[8];
    QList<int> poles;
    QList<QList<int>> polarFaces;
    virtual bool hasCubicSymmetry() const {return false;}
    void getCubicAreas();
    void getPoles();
#endif
private:
    LittlePolyhedron* littlePoly;
    friend class LittlePolyhedron;
    void saveMove(int iface, uchar colorInd);
};
class LittlePolyhedron: public PolyhedronBase
{
public:
    LittlePolyhedron(Polyhedron* bigPoly);
    friend class Polyhedron;
};
#endif // POLYHEDRON_H
