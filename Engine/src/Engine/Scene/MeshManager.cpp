#include "engpch.h"
#include "MeshManager.h"
#include "scene.h"
#include "Engine/Log.h"
#include "assimp/postprocess.h"



Engine::MeshManager::~MeshManager()
{

}

Engine::StaticMesh Engine::MeshManager::GetMesh(const std::string& path)
{
    if (staticMeshes.find(path) != staticMeshes.end())
    {
        return staticMeshes[path];
    }
    ImportMesh(path);
    return (staticMeshes[path]);
}

Engine::Mesh& Engine::MeshManager::GetMeshFromID(uint32_t meshID) const
{
    if(meshID > meshes.size())
    {
        ENGINE_CORE_ASSERT(false, "MESH_ID IS INVALID");
    }
    ENGINE_CORE_ASSERT(meshes[meshID], "NO MESH EXISTS AT INDEX {0}", meshID);
    return *meshes[meshID];
}

Engine::MeshManager::MeshManager()
{

}

void Engine::MeshManager::ImportMesh(const std::string& filepath)
{
    
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        ENGINE_LOG_ERROR("|| ASSIMP || {0}" ,import.GetErrorString());
        return;
    }

    StaticMesh temp;
    currentStaticMesh = temp;
    ProcessNode(scene->mRootNode, scene);
    staticMeshes[filepath] = currentStaticMesh;
}

void Engine::MeshManager::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
	    aiMesh* mesh = scene->mMeshes[i];
	    currentStaticMesh.meshes.emplace_back( ProcessMesh(mesh, scene) );
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

uint32_t Engine::MeshManager::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices ;
    std::vector<uint32_t> indices;

    //Fyller ut vertexer
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 pos(mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z);

        glm::vec3 normal{ 0.f };
        if (mesh->HasNormals())
        {
            normal = glm::vec3(mesh->mNormals[i].x,
								mesh->mNormals[i].y,
								mesh->mNormals[i].z);
        }

        glm::vec2 TextureCoord = glm::vec2(0.f);
        if (mesh->mTextureCoords[0])
        {
            TextureCoord = glm::vec2(mesh->mTextureCoords[0][i].x,
									 mesh->mTextureCoords[0][i].y);
        }

        glm::vec4 color (1.f);
        //
        // if(mesh->HasVertexColors())
        // {
        //     color.r = mesh->mColors[i]->r;
        //     color.g = mesh->mColors[i]->g;
        //     color.b = mesh->mColors[i]->g;
        //     color.a = mesh->mColors[i]->a;
        // }
        
        vertices.push_back(Vertex{ pos,normal, color, TextureCoord});
    }

    //Henter indekser
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    meshes.emplace_back(new Mesh(vertices, indices));
    return meshes.size()-1;

//      TODO: HENTE UT MATERIAL INFO
//      if (scene->HasMaterials())
//      {
//          int idx = mesh->mMaterialIndex;
//          auto mat = scene->mMaterials[idx];
//          if(mat->mNumProperties > 0)
//          {
//              /*
//              glm::vec3 diffuse;
//              glm::vec3 ambient;
//              glm::vec3 specular;
//              glm::vec3 roughness;
//              glm::vec3 emissive;
//              glm::vec4 emissive_color;
//
//              float shininess = 0.f;
//              bool twoSided = false;
//
//         	*/
//
//              aiColor3D col;
//              mat->Get(AI_MATKEY_COLOR_DIFFUSE, col);
//          }
//      }

	
}
