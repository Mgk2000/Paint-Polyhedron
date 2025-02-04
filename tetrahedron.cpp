#include "tetrahedron.h"
#include "cube.h"

Tetrahedron::Tetrahedron(MainWidget *mw) : Polyhedron(mw)
{
    if (mainWidget->gameStartInfo.vertexInfo)
    {
        loadVertexInfo();
        vertexData = new CubeVertexData[faces.length()* 3];
        return;
    }

    radius = 1.0;
#ifdef WIN32
    vertices.append(Vertex(1,1,1));
    vertices.append(Vertex(-1,-1,1));
    vertices.append(Vertex(-1,1,-1));
    vertices.append(Vertex(1,-1,-1));
    for (int i =0; i< vertices.size(); i++)
        vertices[i].vertex = vertices[i].vertex * radius;
    createFacesFromVertices();
    createEdgesFromFaces();
    setDivision(mainWidget->gameStartInfo.division);
    saveVertexInfo();
    vertexData = new CubeVertexData[faces.length()* 3];
#endif
}

LittleTetrahedron::LittleTetrahedron(Tetrahedron *bigTet) : LittlePolyhedron(bigTet)
{
    vertexData = new CubeVertexData[faces.length()* 3];

}
