#pragma once
#include "entt.hpp"
#include "Scene.h"
#include "Engine/Core.h"
#include "glm/glm.hpp"

namespace Engine
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... args>
		T& AddComponent(args&&... _args)
		{
			return m_Scene->GetReg().emplace_or_replace<T>(m_Entity, std::forward<args>(_args));
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->GetReg().get<T>(m_Entity);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->GetReg().any_of<T>(m_Entity);
		}

	private:
		Scene* m_Scene;
		entt::entity m_Entity;
	};
}
