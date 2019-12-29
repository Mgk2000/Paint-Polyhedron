#include "octahedron.h"
#include "cube.h"

Octahedron::Octahedron(MainWidget * mw) : Polyhedron (mw)
{
    if (mainWidget->gameStartInfo.vertexInfo)
    {
        loadVertexInfo();
        vertexData = new CubeVertexData[faces.length()* 3];
        return;
    }
    radius = 1.7f;
    Vertex v0(0,0,-1);
    vertices.append(v0);
    Vertex v1(-1,0,0);
    vertices.append(v1);
    Vertex v2(0,-1,0);
    vertices.append(v2);
    Vertex v3(1,0,0);
    vertices.append(v3);
    Vertex v4(0,1,0);
    vertices.append(v4);
    Vertex v5(0,0,1);
    vertices.append(v5);
    for (int i =0; i< 6; i++)
        vertices[i].vertex = vertices[i].vertex * radius;
    Edge e0(0,1);
    edges.append(e0);
    Edge e1(0,2);
    edges.append(e1);
    Edge e2(0,3);
    edges.append(e2);
    Edge e3(0,4);
    edges.append(e3);
    Edge e4(5,1);
    edges.append(e4);
    Edge e5(5,2);
    edges.append(e5);
    Edge e6(5,3);
    edges.append(e6);
    Edge e7(5,4);
    edges.append(e7);
    Edge e8(1,2);
    edges.append(e8);
    Edge e9(2,3);
    edges.append(e9);
    Edge e10(3,4);
    edges.append(e10);
    Edge e11(4,1);
    edges.append(e11);

    _Face f0(0,1,2);
    faces.append(f0);
    _Face f1( 0,2,3);
    faces.append(f1);
    _Face f2( 0,3,4);
    faces.append(f2);
    _Face f3(0,4,1);
    faces.append(f3);
    _Face f4(5,2,1);
    faces.append(f4);
    _Face f5(5,3,2);
    faces.append(f5);
    _Face f6(5,4,3);
    faces.append(f6);
    _Face f7(5,1,4);
    faces.append(f7);
    setDivision(mainWidget->gameStartInfo.division);
    saveVertexInfo();
    vertexData = new CubeVertexData[faces.length()* 3];
}

LittleOctahedron::LittleOctahedron(Octahedron *bigOct): LittlePolyhedron(bigOct)
{
    vertexData = new CubeVertexData[faces.length()* 3];
}
