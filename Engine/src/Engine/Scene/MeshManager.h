#pragma once

//#include "Renderer/Mesh.h"
#include <string>
#include "Mesh.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"

namespace Engine
{
	class MeshManager
	{
	public:
		static MeshManager& instance()
		{
			static MeshManager instance;
			return instance;
		}
		~MeshManager();
		const Mesh& operator[] (const int& i)
		{
			ENGINE_CORE_ASSERT(i < meshes.size(), "There is not a loaded mesh at index {0}", i);
			return *meshes[i];
		}
		StaticMesh GetMesh(const std::string& path);
		Mesh& GetMeshFromID(uint32_t meshID) const;
	private:
		MeshManager();
		std::unordered_map<std::string, StaticMesh> staticMeshes;
		//std::unordered_map<StaticMesh, std::vector<glm::mat4&>> InstancedMeshes;
		std::vector<Mesh*> meshes;
		StaticMesh currentStaticMesh;

	protected:

		void ImportMesh(const std::string& filepath);
		void ProcessNode(aiNode* node, const aiScene* scene);
		uint32_t ProcessMesh(aiMesh* mesh, const aiScene* scene);
						
	};
}