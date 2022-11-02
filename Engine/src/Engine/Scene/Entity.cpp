#include "engpch.h"
#include "Entity.h"

namespace Engine
{
	Entity::Entity(entt::entity handle, Scene* scene)
	{
		m_Entity = handle;
		m_Scene = scene;

	}
}
