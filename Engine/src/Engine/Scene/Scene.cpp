#include "engpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Engine/Application.h"
#include "Engine/Input.h"
#include "Engine/Renderer/Renderer.h"

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "Engine/KeyCodes.h"
#define SIZE 16.f

namespace Engine
{
	Scene::Scene() 
	{
		physicsWorld = new PhysicsWorld();
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tagName)
	{
		Entity entity = { m_Registry.create(), m_Registry , this};
		entity.AddComponent<Tag>(tagName);
		entity.AddComponent<Transform>(glm::mat4{1.f});
		// entity.AddComponent<StaticMeshRenderer>("../Engine/Assets/3D/sphere.gltf");
		entity.AddComponent<StaticMeshRenderer>("../Engine/Assets/3D/Primitives/Cube_HP.gltf");
		return entity;
	}

	void Scene::OnViewportResize(uint32_t w, uint32_t h)
	{
		viewportHeight = h;
		viewportWidth = w;
		auto view = m_Registry.view<CameraComponent>();
		for(auto& cam : view)
		{
			auto& camera = view.get<CameraComponent>(cam);
			if(!camera.FixedAspectRatio)
				camera.camera->Resize(w, h);
		}
	}

	void Scene::OnUpdate(const double& dt)
	{
		{
			auto view = m_Registry.view<Transform, CameraComponent>();
			for(auto& CamEntity : view)
			{
				auto& [transform, camera] = view.get<Transform, CameraComponent>(CamEntity);
				if (camera.Main && camera.camera)
				{
					camera.camera->SetPosition(transform.position);
					camera.camera->SetRotation(transform.euler_rotation);
				}
				//TODO Set camera with Main = true to the Rendertarget Camera. Potentially adding Rendertarget textures.
			}
		}

		{
			auto view = m_Registry.view<Transform, StaticMeshRenderer>();
			for (auto& entity : view)
			{
				auto& [transform, staticmesh] = view.get<Transform, StaticMeshRenderer>(entity);
				for(int i = 0; i < staticmesh.mesh.meshes.size(); i++)
				{
					auto mesh = MeshManager::instance().GetMeshFromID(staticmesh.mesh.meshes[i]);
					Renderer::Submit(mesh.material, mesh.vertexArray, transform, static_cast<int>(entity));
				}
			}
		}
	}

	void Scene::OnFixedUpdate(const double& dt)
	{
		auto view = m_Registry.view<Transform, Rigidbody>();
		for (auto& entt : view)
		{
			auto& [transform, rigidbody] = view.get<Transform, Rigidbody>(entt);
			btVector3 vec = rigidbody.rigidBody->getWorldTransform().getOrigin();
			transform.position = { vec.x(), vec.y(), vec.z()};
		}
	}
}
