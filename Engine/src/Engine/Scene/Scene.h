#pragma once

#include "entt.hpp"
#include "Engine/Renderer/Camera.h"
#include "Engine/Audio/sound.h"
#include "Engine/Physics/PhysicsWorld.h"
//#include "Engine/Physics/PhysicsWorld.h"

namespace Engine
{
	class Scene
	{
		friend class Entity;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& tagName = "_empty_");

		entt::registry& GetReg() { return m_Registry; }

		void OnViewportResize(uint32_t w, uint32_t h);
		void StartSimulation();
		void StopSimulation();
		void OnUpdate(const double& dt);
		void OnFixedUpdate(const double& dt);

		void SetSimulationState(bool bSimulate) { simulate = bSimulate; }
		bool IsSimulating() const { return simulate; }
	private:
		entt::registry m_Registry;
		double deltaTime;
		PhysicsWorld* physicsWorld;

		bool simulate { false };
		double fixedDTCounter {0};

		uint32_t viewportWidth {0};
		uint32_t viewportHeight {0};

		friend class SceneHierarchyPanel;
		friend class Entity;
		friend class SceneSerializer;
	};

}
