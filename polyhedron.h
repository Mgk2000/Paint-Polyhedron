#ifndef POLYHEDRON_H
#define POLYHEDRON_H
#include "rotatingfigure.h"
class PolyhedronBase;
struct Vertex
{
    Vertex() {}
    Vertex(float _x, float _y, float _z);
    QVector3D vertex;
    QList <int> edges;
    QList <int> faces;
};
struct Edge
{
    Edge (int indA, int indB);
    int vertices[2];
    inline float length() const;
    QList <int> neighborEdges [2];
    int faces[2];
    inline bool isEqual(const Edge& e2);
    bool operator == (const Edge& e2)
        {return isEqual(e2);}
};
struct CubeVertexData;
struct _Face
{
    _Face(PolyhedronBase* _parent, int indA, int indB, int indC);
    const QVector3D& vertex(int ind) const;
    void fillVertexData(CubeVertexData* buf);
    QList <uint> vertices;
    ushort color;
    PolyhedronBase* parent;
};

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
    QList <Edge> edges;
    QList <_Face> faces;
    int nElements() const;
    CubeVertexData* vertexData;
    float radius;
    friend struct _Face;
    int selIndex;
    void saveVertexInfo();
    void loadVertexInfo();
    void setFaceColor(uint nf, int iColor);
    void createFacesFromVertices();
    void createEdgesFromFaces();
    bool edgeExists(int iA, int iB);
};
class LittlePolyhedron;
class Polyhedron: public PolyhedronBase
{
public :
    Polyhedron(MainWidget* mw);
    int pick(float mx, float my, int icolor) override;
    int pick(int nf, int icolor);

    int validColorsCount(RotatingFigure *lf) const override;
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
