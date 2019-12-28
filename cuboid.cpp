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

/*    edges.append(Edge(0,1));  //from 0
    edges.append(Edge(0,3));
    edges.append(Edge(0,4));
    edges.append(Edge(0,8));
    edges.append(Edge(0,10));
    edges.append(Edge(0,12));

    edges.append(Edge(1,2));  //from 1
    edges.append(Edge(1,5));
    edges.append(Edge(1,9));
    edges.append(Edge(1,10));
    edges.append(Edge(1,12));

    edges.append(Edge(2,3));  //from 2
    edges.append(Edge(2,6));
    edges.append(Edge(2,9));
    edges.append(Edge(2,11));
    edges.append(Edge(2,12));

    edges.append(Edge(3,7)); //from 3
    edges.append(Edge(3,8));
    edges.append(Edge(3,11));
    edges.append(Edge(3,12));

    edges.append(Edge(4,5)); //from 4
    edges.append(Edge(4,7));
    edges.append(Edge(4,8));
    edges.append(Edge(4,10));
    edges.append(Edge(4,3));

    edges.append(Edge(5,6)); //from 5
    edges.append(Edge(5,9));
    edges.append(Edge(5,10));
    edges.append(Edge(5,13));

    edges.append(Edge(6,7)); //from 6
    edges.append(Edge(6,9));
    edges.append(Edge(6,11));
    edges.append(Edge(6,13));

    edges.append(Edge(7,8)); //from 7
    edges.append(Edge(7,11));
    edges.append(Edge(7,13));*/

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
