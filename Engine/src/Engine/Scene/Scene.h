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

		std::shared_ptr<Camera> GetActiveCamera() const { return pers_camera; }

		void OnUpdate(const double& dt);

	private:
		void MoveCamera();

	private:
		entt::registry m_Registry;

		std::shared_ptr<PerspectiveCamera> pers_camera;
		std::shared_ptr<OrthographicCamera> orth_camera;
		

		int activeCamera = 0;;
		double deltaTime;
	};

}
