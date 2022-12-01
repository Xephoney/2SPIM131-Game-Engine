#pragma once

#include "Entity.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Audio/sound.h"
#include "Engine/Physics/PhysicsWorld.h"

namespace Engine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& tagName = "_empty_");

		entt::registry& GetReg() { return m_Registry; }

		void OnViewportResize(uint32_t w, uint32_t h);
		void OnUpdate(const double& dt);
		void OnFixedUpdate(const double& dt);
	
	private:
		entt::registry m_Registry;
		double deltaTime;
		PhysicsWorld* physicsWorld;


		uint32_t viewportWidth {0};
		uint32_t viewportHeight {0};
	};

}
