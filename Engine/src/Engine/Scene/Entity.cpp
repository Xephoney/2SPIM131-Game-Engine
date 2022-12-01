#include "engpch.h"
#include "Entity.h"
#include "Scene.h"


namespace Engine
{
	

	Entity::Entity(entt::entity handle, entt::registry& registry, Scene* scene)
		: m_Entity(handle), m_Registry(registry), m_scene(scene)
	{

	}

	
}
