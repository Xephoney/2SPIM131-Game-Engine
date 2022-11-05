#include "engpch.h"
#include "Entity.h"

namespace Engine
{
	

	Entity::Entity(entt::entity handle, entt::registry& registry)
		: m_Entity(handle), m_Registry(registry)
	{

	}

	
}
