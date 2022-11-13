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
		pers_camera = std::make_shared<PerspectiveCamera>(50.f, (16.f / 9.f), 0.01f, 1000.f);
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& tagName)
	{
		Entity entity = { m_Registry.create(), m_Registry };
		entity.AddComponent<Tag>(tagName);
		entity.AddComponent<Transform>(glm::mat4{1.f});
		//entity.AddComponent<StaticMeshRenderer>("../Engine/Assets/3D/Primitives/sphere.fbx");
		entity.AddComponent<StaticMeshRenderer>("../Engine/Assets/3D/coin.gltf");
		return entity;
	}

	void Scene::OnUpdate(const double& dt)
	{
		deltaTime = dt;
		MoveCamera();
		GetActiveCamera()->update(dt);
		// auto view = m_Registry.view<Transform, MeshRenderer, Material>();
		//
		// for(const auto e : view)
		// {
		// 	auto& [transform, mesh, material] = view.get<Transform, MeshRenderer, Material>(e);
		// 	Renderer::Submit(material, mesh, transform);
		// }
		// auto group = m_Registry.group<Transform>(entt::get<MeshRenderer>);


		auto view2 = m_Registry.view<Transform, StaticMeshRenderer>();

		for (auto entity : view2)
		{
			auto& [transform, staticmesh] = view2.get<Transform, StaticMeshRenderer>(entity);
			for(int i = 0; i < staticmesh.mesh.meshes.size(); i++)
			{
				auto mesh = MeshManager::instance().GetMeshFromID(staticmesh.mesh.meshes[i]);
				Renderer::Submit(mesh.material, mesh.vertexArray, transform);
			}
		}
		// ENGINE_LOG_INFO("entt's found {0} of {1} alive", counter, m_Registry.alive());
		// Camera Movement
		
	}

	void Scene::MoveCamera()
	{
		std::shared_ptr<Camera> camera = GetActiveCamera();

		glm::vec3 movement_direction {0,0,0};
		if(Input::IsKeyPressed(KEY_W))
		{
			movement_direction += camera->Forward();
		}
		if (Input::IsKeyPressed(KEY_A))
		{
			movement_direction -= camera->Right();
		}
		if (Input::IsKeyPressed(KEY_S))
		{
			movement_direction -= camera->Forward();
		}
		if (Input::IsKeyPressed(KEY_D))
		{
			movement_direction += camera->Right();
		}
		if (Input::IsKeyPressed(KEY_Q))
		{
			movement_direction -= camera->Up();
		}
		if (Input::IsKeyPressed(KEY_E))
		{
			movement_direction += camera->Up();
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
					camera->Direction() = glm::normalize(newDir);
				}

				camera->Direction() = glm::normalize(glm::rotate(camera->Direction(), glm::radians(-delta.x * cameraSensitivity * static_cast<float>(deltaTime)), { 0, 1, 0 }));
				if (glm::length(movement_direction) > 0.01)
				{
					movement_direction = glm::normalize(movement_direction);
					camera->m_movementDir = movement_direction;
				}
			}

			bFirstClick = false;
			Input::SetMousePosition({ center.x, center.y });
		}
		else
			bFirstClick = true;
	}
}
