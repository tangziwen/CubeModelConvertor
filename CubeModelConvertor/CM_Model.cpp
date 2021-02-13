#include "CM_Model.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/error/en.h>
namespace tzw {
CM_Model::CM_Model(GLchar *path)
{
    this->loadModel(path);
    this->model_path = path;
}
std::string CM_Model::getFileNameWithOutExtension(std::string path)
{
	auto idx = path.rfind('.');

	if(idx != std::string::npos)
	{
	    return path.substr(0,idx);
	}
	else
	{
	    // No extension found
		return path;
	}
}
void CM_Model::DumpTo(string targetPos, float scale)
{
    rapidjson::Document document;
    document.SetObject();
    auto& allocator = document.GetAllocator();


    rapidjson::Document MatDocument;
	MatDocument.SetObject();
    dumpMetaInfo(document);
	auto modelName = getFileNameWithOutExtension(targetPos);
    std::string matFileName = modelName + ".matList";
    printf("m_materials size %d\n",m_materials.size());
    if (m_materials.size()> 0)
    {

    	auto& matAllocator = MatDocument.GetAllocator();
        rapidjson::Value materialArray(rapidjson::kArrayType);
    	rapidjson::Value matExtraArray(rapidjson::kArrayType);
        for(auto& material : m_materials)
        {
            auto result = dumpMaterial(material,document);
            materialArray.PushBack(result,allocator);

        	auto resultExtra = dumpMaterialSTD(modelName, material,document);
        	matExtraArray.PushBack(resultExtra, matAllocator);
        }
        document.AddMember("materialList", materialArray, allocator);
    	document.AddMember("MaterialsFileName", matFileName, allocator);
    	MatDocument.AddMember("MaterialList", matExtraArray, matAllocator);
    }
    rapidjson::Value meshArray(rapidjson::kArrayType);
    for(auto& mesh : meshes)
    {
        auto result = dumpMesh(mesh,document, scale);
        meshArray.PushBack(result,allocator);
    }
    document.AddMember("MeshList", meshArray, allocator);
    rapidjson::StringBuffer  buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    FILE* file = fopen(targetPos.c_str(), "wb");
    if (file)
    {
        fputs(buffer.GetString(), file);
        fclose(file);
    }
    FILE* matFile = fopen(matFileName.c_str(), "wb");
    if(matFile)
    {
	    rapidjson::StringBuffer  matBuffer;
	    rapidjson::PrettyWriter<rapidjson::StringBuffer> matWriter(matBuffer);
    	MatDocument.Accept(matWriter);
        fputs(matBuffer.GetString(), matFile);
        fclose(matFile);
    }
}

void CM_Model::dumpMetaInfo(rapidjson::Document &doc)
{
    auto& allocator = doc.GetAllocator();
    rapidjson::Value info(rapidjson::kObjectType);
    info.AddMember("author","tzw",allocator);
    info.AddMember("meshAmount",this->meshes.size(),allocator);
    info.AddMember("materialAmount",this->m_materials.size(),allocator);
    doc.AddMember("info",info,allocator);
}

rapidjson::Value CM_Model::dumpMesh(CM_Mesh &mesh,rapidjson::Document &doc, float scale)
{
    auto& allocator = doc.GetAllocator();
    rapidjson::Value theMesh(rapidjson::kObjectType);
    rapidjson::Value verticesArray(rapidjson::kArrayType);
    rapidjson::Value indicesArray(rapidjson::kArrayType);
    for (auto& vertex : mesh.vertices)
    {
        rapidjson::Value vertexAttribute(rapidjson::kArrayType);

        //pos
        vertexAttribute.PushBack(vertex.Position.x * scale,allocator);
        vertexAttribute.PushBack(vertex.Position.y * scale,allocator);
        vertexAttribute.PushBack(vertex.Position.z * scale,allocator);

        //normal
        vertexAttribute.PushBack(vertex.Normal.x,allocator);
        vertexAttribute.PushBack(vertex.Normal.y,allocator);
        vertexAttribute.PushBack(vertex.Normal.z,allocator);

        //UV0
        vertexAttribute.PushBack(vertex.texCoordsUV0.x,allocator);
        vertexAttribute.PushBack(vertex.texCoordsUV0.y,allocator);

        verticesArray.PushBack(vertexAttribute,allocator);
    }
    for (auto index :mesh.indices)
    {
        indicesArray.PushBack(index,allocator);
    }
    theMesh.AddMember("vertices",verticesArray,allocator);
    theMesh.AddMember("indices",indicesArray,allocator);
    theMesh.AddMember("materialIndex",mesh.materialIndex,allocator);
    return theMesh;
}
#define ADDSTR(OBJ,FIELD,STR,ALLOC) OBJ.AddMember(FIELD,rapidjson::Value(STR.c_str(),ALLOC),ALLOC)
#define ADDCSTR(OBJ,FIELD,STR,ALLOC) OBJ.AddMember(FIELD,rapidjson::Value(STR,ALLOC),ALLOC)
rapidjson::Value CM_Model::dumpMaterial(CM_Material &mat, rapidjson::Document &doc)
{
    auto& allocator = doc.GetAllocator();
    rapidjson::Value material(rapidjson::kObjectType);
    ADDSTR(material,"diffuseMap",mat.diffuseMap,allocator);
    ADDSTR(material,"specularMap",mat.specularMap,allocator);
    ADDSTR(material,"ambientMap",mat.ambientMap,allocator);
    ADDSTR(material,"emissiveMap",mat.emissiveMap,allocator);
    ADDSTR(material,"heightMap",mat.heightMap,allocator);
    ADDSTR(material,"normalMap",mat.normalMap,allocator);
    ADDSTR(material,"shininessMap",mat.shininessMap,allocator);
    ADDSTR(material,"opacityMap",mat.opacityMap,allocator);
    ADDSTR(material,"displacementMap",mat.displacementMap,allocator);
    ADDSTR(material,"lightMap",mat.lightMap,allocator);
    ADDSTR(material,"reflectionMap",mat.reflectionMap,allocator);
    return material;
}

rapidjson::Value genV4(float x, float y, float z, float w, rapidjson::Document &doc)
{
	auto& allocator = doc.GetAllocator();
    rapidjson::Value attributeNode(rapidjson::kArrayType);
	attributeNode.PushBack(x, allocator);
	attributeNode.PushBack(y, allocator);
	attributeNode.PushBack(z, allocator);
	attributeNode.PushBack(w, allocator);
	return attributeNode;
}


rapidjson::Value CM_Model::dumpMaterialSTD(std::string name, CM_Material &mat, rapidjson::Document &doc)
{
    auto& allocator = doc.GetAllocator();
    rapidjson::Value material(rapidjson::kObjectType);

	//shader
    rapidjson::Value shaders(rapidjson::kObjectType);
	ADDCSTR(shaders, "vs","Shaders/GeometryPass_v.glsl",allocator);
	ADDCSTR(shaders, "fs","Shaders/GeometryPassPBR_f.glsl",allocator);
    material.AddMember("shaders", shaders, allocator);
	
    ADDCSTR(material, "name", name.c_str(), allocator);


	rapidjson::Value propertyNode(rapidjson::kObjectType);
	rapidjson::Value attributeNode(rapidjson::kArrayType);
	rapidjson::Value mapNode(rapidjson::kArrayType);

	rapidjson::Value TU_camPos(rapidjson::kObjectType);
	rapidjson::Value TU_roughness(rapidjson::kObjectType);
	rapidjson::Value TU_color(rapidjson::kObjectType);
	
	ADDCSTR(TU_camPos, "name","TU_camPos",allocator);
	ADDCSTR(TU_camPos, "type","semantic_CameraPos",allocator);

	ADDCSTR(TU_roughness, "name","TU_roughness",allocator);
	ADDCSTR(TU_roughness, "type","float",allocator);
	TU_roughness.AddMember("default", 1.0, allocator);

	ADDCSTR(TU_color, "name","TU_color",allocator);
	ADDCSTR(TU_color, "type","vec4",allocator);
    TU_color.AddMember("default", genV4(1, 1, 1, 1, doc), allocator);

	attributeNode.PushBack(TU_camPos, allocator);
	attributeNode.PushBack(TU_roughness, allocator);
	attributeNode.PushBack(TU_color, allocator);

    rapidjson::Value DiffuseMap(rapidjson::kArrayType);
	DiffuseMap.PushBack("DiffuseMap", allocator);
	DiffuseMap.PushBack(0, allocator);
	DiffuseMap.PushBack(rapidjson::Value(mat.diffuseMap.c_str(),allocator), allocator);
	
	rapidjson::Value MetallicMap(rapidjson::kArrayType);
	MetallicMap.PushBack("MetallicMap", allocator);
	MetallicMap.PushBack(1, allocator);
	
	rapidjson::Value RoughnessMap(rapidjson::kArrayType);
	RoughnessMap.PushBack("RoughnessMap", allocator);
	RoughnessMap.PushBack(2, allocator);
	
	rapidjson::Value NormalMap(rapidjson::kArrayType);
	NormalMap.PushBack("NormalMap", allocator);
	NormalMap.PushBack(3, allocator);

	mapNode.PushBack(DiffuseMap, allocator);
	mapNode.PushBack(MetallicMap, allocator);
	mapNode.PushBack(RoughnessMap, allocator);
	mapNode.PushBack(NormalMap, allocator);

    propertyNode.AddMember("attributes", attributeNode, allocator);
	propertyNode.AddMember("maps", mapNode, allocator);

	material.AddMember("property",propertyNode, allocator);
	
	// material.AddMember()
    // ADDSTR(material,"diffuseMap",mat.diffuseMap,allocator);
    // ADDSTR(material,"specularMap",mat.specularMap,allocator);
    // ADDSTR(material,"ambientMap",mat.ambientMap,allocator);
    // ADDSTR(material,"emissiveMap",mat.emissiveMap,allocator);
    // ADDSTR(material,"heightMap",mat.heightMap,allocator);
    // ADDSTR(material,"normalMap",mat.normalMap,allocator);
    // ADDSTR(material,"shininessMap",mat.shininessMap,allocator);
    // ADDSTR(material,"opacityMap",mat.opacityMap,allocator);
    // ADDSTR(material,"displacementMap",mat.displacementMap,allocator);
    // ADDSTR(material,"lightMap",mat.lightMap,allocator);
    // ADDSTR(material,"reflectionMap",mat.reflectionMap,allocator);
    return material;
}
void CM_Model::processMaterials(const aiScene *scene)
{
    int num = scene->mNumMaterials;
    CM_Material theMat;
    for(int i =0; i < num; i++)
    {
        auto mat = scene->mMaterials[i];
        aiString tmpStr;
        //读取若干贴图文件
        if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            mat->GetTexture(aiTextureType_DIFFUSE,0,&tmpStr);
            theMat.diffuseMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0)
        {
            mat->GetTexture(aiTextureType_SPECULAR,0,&tmpStr);
            theMat.specularMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_AMBIENT) > 0)
        {
            mat->GetTexture(aiTextureType_AMBIENT,0,&tmpStr);
            theMat.ambientMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_EMISSIVE) > 0)
        {
            mat->GetTexture(aiTextureType_EMISSIVE,0,&tmpStr);
            theMat.emissiveMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_HEIGHT) > 0)
        {
            mat->GetTexture(aiTextureType_HEIGHT,0,&tmpStr);
            theMat.heightMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_NORMALS) > 0)
        {
            mat->GetTexture(aiTextureType_NORMALS,0,&tmpStr);
            theMat.normalMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_SHININESS) > 0)
        {
            mat->GetTexture(aiTextureType_SHININESS,0,&tmpStr);
            theMat.shininessMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_OPACITY) > 0)
        {
            mat->GetTexture(aiTextureType_OPACITY,0,&tmpStr);
            theMat.opacityMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_DISPLACEMENT) > 0)
        {
            mat->GetTexture(aiTextureType_DISPLACEMENT,0,&tmpStr);
            theMat.displacementMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_LIGHTMAP) > 0)
        {
            mat->GetTexture(aiTextureType_LIGHTMAP,0,&tmpStr);
            theMat.lightMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_REFLECTION) > 0)
        {
            mat->GetTexture(aiTextureType_REFLECTION,0,&tmpStr);
            theMat.reflectionMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
        {
            mat->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS,0,&tmpStr);
            theMat.roughnessMap = tmpStr.C_Str();
        }
        if (mat->GetTextureCount(aiTextureType_METALNESS) > 0)
        {
            mat->GetTexture(aiTextureType_METALNESS,0,&tmpStr);
            theMat.metallicMap = tmpStr.C_Str();
        }
    	m_materials.push_back(theMat);
    }
}


void CM_Model::loadModel(string path)
{
    // Read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices);
    // Check for errors
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    // Retrieve the directory path of the filepath
    this->directory = path.substr(0, path.find_last_of('/'));

    // Process ASSIMP's root node recursively
    this->processNode(scene->mRootNode, scene);
    this->processMaterials(scene);
}

void CM_Model::processNode(aiNode *node, const aiScene *scene)
{
    // Process each mesh located at the current node
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        // The node object only contains indices to index the actual objects in the scene.
        // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->processMesh(mesh, scene));
    }
    // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }

}

CM_Mesh CM_Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // Data to fill
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    // Walk through each of the mesh's vertices
    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        v3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // Positions
        vector = v3 (mesh->mVertices[i].x, mesh->mVertices[i].y,mesh->mVertices[i].z);
        vertex.Position = vector;
        // Normals
        vector = v3 (mesh->mNormals[i].x, mesh->mNormals[i].y,mesh->mNormals[i].z);
        vertex.Normal = vector;
        // Texture Coordinates
        if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
        {
            v2 vec;
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec = v2(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
            vertex.texCoordsUV0 = vec;
        }
        else
            vertex.texCoordsUV0 = v2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // Retrieve all indices of the face and store them in the indices vector
        for(GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // Process materials
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // Diffuse: texture_diffuseN
        // Specular: texture_specularN
        // Normal: texture_normalN

        // 1. Diffuse maps
        vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. Specular maps
        vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    printf("the Material Index %d %d\n", scene->mNumMaterials,  mesh->mMaterialIndex);
    // Return a mesh object created from the extracted mesh data
    return CM_Mesh(vertices, indices, textures,mesh->mMaterialIndex);
}

vector<Texture> CM_Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(GLuint j = 0; j < textures_loaded.size(); j++)
        {
            if(textures_loaded[j].path.compare(str.C_Str()))
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // If texture hasn't been loaded already, load it
            Texture texture;
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

} // namespace tzw
