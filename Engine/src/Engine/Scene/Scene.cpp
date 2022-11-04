#include "engpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Engine/Input.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& tagName)
	{
		Entity entity = { m_Registry.create(), m_Registry };
		entity.AddComponent<Tag>(tagName);
		entity.AddComponent<Transform>(glm::mat4{1.f});
		entity.AddComponent<MeshRenderer>();
		entity.AddComponent<Material>();
		return entity;
	}

	void Scene::OnUpdate(const double& dt)
	{
		auto view = m_Registry.view<Transform, MeshRenderer, Material>();
		
		for(const auto e : view)
		{
			auto& [transform, mesh, material] = view.get<Transform, MeshRenderer, Material>(e);
			Renderer::Submit(material, mesh, transform);
		}
		
		auto group = m_Registry.group<Transform>(entt::get<MeshRenderer>);
		// int counter = 0;
		/*for(auto& entity : group)
		{
			auto& [transform, mesh] = group.get<Transform, MeshRenderer>(entity);
			Renderer::Submit(mesh.vertexArray);
			// counter++;
		}*/
		// ENGINE_LOG_INFO("entt's found {0} of {1} alive", counter, m_Registry.alive());
		// Camera Movement

	}
}
