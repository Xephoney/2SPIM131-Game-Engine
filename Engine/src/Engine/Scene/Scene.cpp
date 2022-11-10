#include "engpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Engine/Application.h"
#include "Engine/Input.h"
#include "Engine/Renderer/Renderer.h"

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

#define SIZE 16.f

namespace Engine
{
	Scene::Scene() 
	{
		//orth_camera = std::make_shared<OrthographicCamera>((-SIZE / 2.f) * (16.f / 9.f), (SIZE / 2.f) * (16.f / 9.f), -(SIZE / 2.f), (SIZE / 2.f));
		pers_camera = std::make_shared<PerspectiveCamera>(90.f, (16.f / 9.f), 0.05f, 1250.f);
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& tagName)
	{
		Entity entity = { m_Registry.create(), m_Registry };
		entity.AddComponent<Tag>(tagName);
		entity.AddComponent<Transform>(glm::mat4{1.f});
		entity.AddComponent<MeshRenderer>();
		entity.AddComponent<Material>();
		entity.AddComponent<AudioListener>();
		return entity;
	}

	void Scene::OnUpdate(const double& dt)
	{
		deltaTime = dt;
		MoveCamera();

		auto view = m_Registry.view<Transform, MeshRenderer, Material>();
		
		for(const auto e : view)
		{
			auto& [transform, mesh, material] = view.get<Transform, MeshRenderer, Material>(e);
			Renderer::Submit(material, mesh, transform);
		}
		auto group = m_Registry.group<Transform>(entt::get<MeshRenderer>);

		// ENGINE_LOG_INFO("entt's found {0} of {1} alive", counter, m_Registry.alive());
		// Camera Movement
		GetActiveCamera()->update(dt);
	}

	void Scene::MoveCamera()
	{


		std::shared_ptr<Camera> camera = GetActiveCamera();

		glm::vec3 direction {0,0,0};
		if(Input::IsKeyPressed(KEY_W))
		{
			direction += camera->Forward();
		}
		if (Input::IsKeyPressed(KEY_A))
		{
			direction -= camera->Right();
		}
		if (Input::IsKeyPressed(KEY_S))
		{
			direction -= camera->Forward();
		}
		if (Input::IsKeyPressed(KEY_D))
		{
			direction += camera->Right();
		}
		if (Input::IsKeyPressed(KEY_Q))
		{
			direction -= camera->Up();
		}
		if (Input::IsKeyPressed(KEY_E))
		{
			direction += camera->Up();
		}


		static bool bFirstClick = true;
		static const glm::vec2 center{ 1280.f / 2.f, 720 / 2.f };
		glm::vec2 mousePos;
		glm::vec2 delta;
		const float cameraSensitivity = 25.f;
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_2))
		{
			if (!bFirstClick)
			{
				auto [x, y] = Input::GetMousePosition();
				mousePos.x = x;
				mousePos.y = y;
				delta = mousePos - center;

				glm::vec3 newDir = glm::rotate(camera->Direction(), glm::radians(-delta.y * cameraSensitivity * static_cast<float>(deltaTime)), GetActiveCamera()->Right());
				if (!(glm::angle(newDir, { 0,1,0 }) <= glm::radians(5.f) || glm::angle(newDir, { 0,-1,0 }) <= glm::radians(5.f)))
				{
					camera->Direction() = newDir;
				}

				camera->Direction() = glm::rotate(camera->Direction(), glm::radians(-delta.x * cameraSensitivity * static_cast<float>(deltaTime)), { 0, 1, 0 });
				if (glm::length(direction) > 0.01)
				{
					direction = glm::normalize(direction);
					camera->m_movementDir = direction;
				}
			}

			bFirstClick = false;
			Input::SetMousePosition({ center.x, center.y });
		}
	}
}
