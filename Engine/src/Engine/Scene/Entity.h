#pragma once
#include "entt.hpp"
#include "Scene.h"

namespace Engine
{
	class Entity
	{
	public:
		Entity() = default;

		Entity(entt::entity handle, Scene* scene)
			: m_EntityHandle(handle), m_Scene(scene)
		{

		}
		Entity(const Entity& other) = default;

		template <typename T, typename ... Args>
		T& AddComponent(Args&&... _args)
		{
			return m_Scene->m_Registry.get_or_emplace<T>(m_EntityHandle, std::forward<Args>(_args)...);
		}

		template <typename T>
		T& GetComponent()
		{
			ENGINE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template <typename T>
		bool HasComponent() const
		{
			if(m_EntityHandle != entt::null)
				return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
			return false;
		}

		template <typename T>
		void RemoveComponent() const
		{
			ENGINE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		bool Destroy()
		{
			m_Scene->m_Registry.destroy(m_EntityHandle);
			m_EntityHandle = entt::null;
			return true;
		}

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}


		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }
	
		entt::entity m_EntityHandle {entt::null};
		Entity* m_parent{ nullptr };
		std::vector<Entity> children;
	private:

		Scene* m_Scene { nullptr };
		
		
		
	};

	
}
