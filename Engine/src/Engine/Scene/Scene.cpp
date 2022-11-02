#include "engpch.h"
#include "Scene.h"
#include "Components.h"

namespace Engine
{
	Scene::Scene()
	{
		entt::entity Camera = m_Registry.create();
		m_Registry.emplace<Transform>(Camera, glm::mat4{ 1.f });
		

	}

	Scene::~Scene()
	{
	}

	entt::entity Scene::CreateEntity()
	{
		auto entity = m_Registry.create();
		m_Registry.emplace<Transform>(entity, glm::mat4{ 1.f });
		return entity;
	}

	void Scene::OnUpdate(const double& dt)
	{
		auto view = m_Registry.view<Transform>();
		for(auto& e : view)
		{
			
		}
	}
}
