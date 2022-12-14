#pragma once

#include "entt.hpp"
#include "Engine/Renderer/Camera.h"
#include "Engine/Audio/sound.h"
#include "Engine/Particle/ParticleSystem.h"
#include "Engine/Physics/PhysicsWorld.h"
//#include "Engine/Physics/PhysicsWorld.h"

namespace Engine
{
	class Scene
	{
		
	public:
		friend class Entity;
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& tagName = "_empty_");
		Entity CreateEmptyEntity(const std::string& tagName = "_empty_");
		Entity CreateEmptyEmitterEntity(const std::string& tagName);

		bool DeleteEntity(Entity& entity);
		bool DeleteEntity(uint32_t entity);

		entt::registry& GetReg() { return m_Registry; }

		void OnViewportResize(uint32_t w, uint32_t h);

		void StartSimulation();
		void StopSimulation();

		void OnUpdate(const double& dt);
		void OnFixedUpdate(const double& dt);

		void ShadowPass();

		void SetSimulationState(bool bSimulate) { simulate = bSimulate; }
		bool IsSimulating() const { return simulate; }


		
	private:

		
		entt::registry m_Registry;
		double deltaTime;
		PhysicsWorld* physicsWorld;
		ParticleSystem* particleSystem;

		bool simulate { false };
		double fixedDTCounter {0};

		uint32_t viewportWidth {0};
		uint32_t viewportHeight {0};

		entt::entity audioSourceID;
		
		friend class SceneHierarchyPanel;
		
		friend class SceneSerializer;

		friend class EditorLayer;
		
	};

	
}
