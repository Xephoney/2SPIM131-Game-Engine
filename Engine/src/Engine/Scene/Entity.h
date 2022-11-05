#pragma once
#include "entt.hpp"

namespace Engine
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, entt::registry& registry);
		Entity(const Entity& other) = default;

		template <typename T, typename ... Args>
		T& AddComponent(Args&&... _args)
		{
			return m_Registry.emplace<T>(m_Entity, std::forward<Args>(_args)...);
		}

		template <typename T>
		T& GetComponent()
		{
			return m_Registry.get<T>(m_Entity);
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_Registry.any_of<T>(m_Entity);
		}

		operator bool() const { return m_Entity != entt::null; }

	private:
		entt::registry& m_Registry;
		entt::entity m_Entity {entt::null};
	};

	
}