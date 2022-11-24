#include "engpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Engine/Application.h"
#include "Engine/Input.h"
#include "Engine/Renderer/Renderer.h"

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "Engine/KeyCodes.h"
#define SIZE 16.f

namespace Engine
{
	Scene::Scene() 
	{
		physicsWorld = new PhysicsWorld();
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tagName)
	{
		Entity entity = { m_Registry.create(), m_Registry };
		entity.AddComponent<Tag>(tagName);
		entity.AddComponent<Transform>(glm::mat4{1.f});
		entity.AddComponent<StaticMeshRenderer>("../Engine/Assets/3D/sphere.gltf");
		return entity;
	}

	void Scene::OnUpdate(const double& dt)
	{
		auto view = m_Registry.view<Transform, StaticMeshRenderer>();
		for (auto& entity : view)
		{
			auto& [transform, staticmesh] = view.get<Transform, StaticMeshRenderer>(entity);
			for(int i = 0; i < staticmesh.mesh.meshes.size(); i++)
			{
				auto mesh = MeshManager::instance().GetMeshFromID(staticmesh.mesh.meshes[i]);
				Renderer::Submit(mesh.material, mesh.vertexArray, transform, static_cast<int>(entity));
			}
		}
	}
}
