#include "cuboid.h"
#include "cube.h"
#include <QtMath>

Cuboid::Cuboid(MainWidget * mw): Polyhedron (mw)

{
    if (mainWidget->gameStartInfo.vertexInfo)
    {
        loadVertexInfo();
        vertexData = new CubeVertexData[faces.length()* 3];
        return;
    }
    radius = 1.0f;
    float r3 = qSqrt(3.0);
    vertices.append(Vertex(-1 ,-1,-1)); //0
    vertices.append(Vertex( 1 ,-1,-1)); //1
    vertices.append(Vertex( 1 , 1,-1)); //2
    vertices.append(Vertex(-1 , 1,-1));  //3
    vertices.append(Vertex(-1 ,-1, 1));  //4
    vertices.append(Vertex( 1 ,-1, 1));  //5
    vertices.append(Vertex( 1 , 1, 1));  //6
    vertices.append(Vertex(-1 , 1, 1));  //7

    vertices.append(Vertex(-r3 , 0, 0));  //8
    vertices.append(Vertex(r3 , 0, 0));   //9
    vertices.append(Vertex(0, -r3 ,  0)); //10
    vertices.append(Vertex(0, r3 ,  0));  //11
    vertices.append(Vertex( 0, 0, -r3));  //12
    vertices.append(Vertex( 0, 0, r3));   //13
    for (int i =0; i< vertices.count(); i++)
        vertices[i].vertex = vertices[i].vertex * radius;

    createFacesFromVertices();
    createEdgesFromFaces();
    setDivision(mainWidget->gameStartInfo.division-1);
    saveVertexInfo();
    vertexData = new CubeVertexData[faces.length()* 3];
}

LittleCuboid::LittleCuboid(Cuboid *bigCuboid) : LittlePolyhedron(bigCuboid)
{
    vertexData = new CubeVertexData[faces.length()* 3];
}
