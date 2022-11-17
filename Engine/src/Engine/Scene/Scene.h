#pragma once

#include "Entity.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Audio/sound.h"

namespace Engine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& tagName = "_empty_");

		entt::registry& GetReg() { return m_Registry; }

		void OnUpdate(const double& dt);
	
	private:
		entt::registry m_Registry;
		double deltaTime;
	};

}
