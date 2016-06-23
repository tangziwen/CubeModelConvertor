#ifndef TZW_CM_MESH_H
#define TZW_CM_MESH_H
#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
typedef unsigned int GLuint;
// GL Includes

namespace tzw {
struct v3{
    v3(float x,float y,float z);
    v3();
    float x,y,z;
};
struct v2{
    v2(float x,float y);
    v2();
    float x,y;
};

struct Vertex {
    // Position
    v3 Position;
    // Normal
    v3 Normal;
    // TexCoords
    v2 texCoordsUV0;
};
struct Texture {
    string type;
    string path;
};
class CM_Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    unsigned int materialIndex;
    /*  Functions  */
    // Constructor
    CM_Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures,unsigned int theMaterialIndex);
private:
};

} // namespace tzw

#endif // TZW_CM_MESH_H
