#pragma once

#include "entt.hpp"

namespace Engine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		entt::registry& GetReg() { return m_Registry; }

		void OnUpdate(const double& dt);

	private:
		entt::registry m_Registry;

	};

}
