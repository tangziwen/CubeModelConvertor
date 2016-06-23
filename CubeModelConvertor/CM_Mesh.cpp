#include "CM_Mesh.h"

namespace tzw {

CM_Mesh::CM_Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, unsigned int theMaterialIndex)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->materialIndex = theMaterialIndex;
}

v3::v3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

v3::v3()
{
    x = y = z = 0;
}

v2::v2(float x, float y)
{
    this->x = x;
    this->y = y;
}

v2::v2()
{
    x = y = 0;
}

} // namespace tzw
