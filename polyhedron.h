#ifndef POLYHEDRON_H
#define POLYHEDRON_H
#include "rotatingfigure.h"
class Polyhedron;
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
};
struct CubeVertexData;
struct _Face
{
    _Face(Polyhedron* _parent, int indA, int indB, int indC);
    const QVector3D& vertex(int ind) const;
    void fillVertexData(CubeVertexData* buf);
    QList <uint> vertices;
    ushort color;
    Polyhedron* parent;
};

class Polyhedron : public RotatingFigure
{
public:
    Polyhedron(MainWidget * mw, bool _little);
    virtual ~Polyhedron();
    void init() override;
    void draw() override;
    void setData(const uchar *data) override;
    int pick(float mx, float my, int icolor) override;
    void fillData() override;
    int validColorsCount (RotatingFigure* lf) override;
    void initGL(QOpenGLShaderProgram* prog) override;
    void setDivision();
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
};

#endif // POLYHEDRON_H
