#include "icosahedron.h"
#include "cube.h"

Icosahedron::Icosahedron(MainWidget *mw): Polyhedron (mw)
{
    if (mainWidget->gameStartInfo.vertexInfo)
    {
        loadVertexInfo();
        vertexData = new CubeVertexData[faces.length()* 3];
        return;
    }    radius = 1.6;
    vertices.append(Vertex(0.809f, 0.5f, 0.588f));
    vertices.append(Vertex(0.309f, -0.5f, 0.951f));
    vertices.append(Vertex(-0.309f, 0.5f, 0.951f));
    vertices.append(Vertex(-0.809f, -0.5f, 0.588f)); //4
    vertices.append(Vertex(-1, 0.5f, 0));
    vertices.append(Vertex(-0.809f, -0.5f, -0.588f));
    vertices.append(Vertex(-0.309f, 0.5f, -0.951f));
    vertices.append(Vertex(0.309f, -0.5f, -0.951f)); //8
    vertices.append(Vertex(0.809f, 0.5f, -0.588f));
    vertices.append(Vertex(1, -0.5f, 0));
    vertices.append(Vertex(0 , 1.118f, 0));
    vertices.append(Vertex(0 , -1.118f, 0));
    for (int i =0; i< vertices.size(); i++)
        vertices[i].vertex = vertices[i].vertex * radius;
    createFacesFromVertices();
    createEdgesFromFaces();
    setDivision(mainWidget->gameStartInfo.division);
    saveVertexInfo();
    vertexData = new CubeVertexData[faces.length()* 3];

}

LittleIcosahedron::LittleIcosahedron(Icosahedron *bigIcos) : LittlePolyhedron(bigIcos)
{
    vertexData = new CubeVertexData[faces.length()* 3];

}
