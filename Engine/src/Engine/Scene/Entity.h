#pragma once
#include "entt.hpp"

namespace Engine
{
	class Scene;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, entt::registry& registry, Scene* scene);
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

		bool operator ==(const Entity& cmp) const
		{
			return m_Entity == cmp.m_Entity && m_scene == cmp.m_scene;
		}
		bool operator !=(const Entity& cmp) const
		{
			return !(*this == cmp);
		}

		operator uint32_t() const { return static_cast<uint32_t>(m_Entity); }
	private:
		entt::registry& m_Registry;
		entt::entity m_Entity {entt::null};
		Scene* m_scene{ nullptr };
		Entity* parent { nullptr };
		std::vector<Entity*> children;
	};

	
}
