#ifndef TZW_CM_MODEL_H
#define TZW_CM_MODEL_H
#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
using namespace std;
// GL Includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
typedef int GLint;
typedef char GLchar;
#include "CM_Mesh.h"

namespace tzw {
struct CM_Material
{
    std::string diffuseMap;
    std::string specularMap;
    std::string ambientMap;
    std::string emissiveMap;
    std::string heightMap;
    std::string normalMap;
    std::string shininessMap;
    std::string opacityMap;
    std::string displacementMap;
    std::string lightMap;
    std::string reflectionMap;
	std::string roughnessMap;
	std::string metallicMap;
};

class CM_Model
{
public:
    /*  Functions   */
    // Constructor, expects a filepath to a 3D model.
    CM_Model(GLchar* path);
    void DumpTo(std::string targetPos, float scale = 1.0);
	std::string getFileNameWithOutExtension(std::string path);
private:
    std::string model_path;
    /*  Model Data  */
    vector<CM_Mesh> meshes;
    vector<CM_Material> m_materials;
    string directory;
    vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    /*  Functions   */
    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string path);
    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);
    CM_Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    void dumpMetaInfo(rapidjson::Document & doc);
    rapidjson::Value dumpMesh(CM_Mesh& mesh,rapidjson::Document &doc, float scale);
    rapidjson::Value dumpMaterial(CM_Material& mat, rapidjson::Document & doc);
	rapidjson::Value dumpMaterialSTD(std::string name, CM_Material& mat, rapidjson::Document & doc);
    void processMaterials(const aiScene* scene);
};


} // namespace tzw

#endif // TZW_CM_MODEL_H
