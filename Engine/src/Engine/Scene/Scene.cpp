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

		CreateEntity("Box");
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tagName)
	{
		Entity entity = { m_Registry.create(), this};
		entity.AddComponent<Tag>(tagName);
		entity.AddComponent<Transform>(glm::mat4{1.f});
		// entity.AddComponent<StaticMeshRenderer>("../Engine/Assets/3D/sphere.gltf");
		entity.AddComponent<StaticMeshRenderer>("../Engine/Assets/3D/Primitives/split_cube.gltf");
#ifdef _DEBUG
		entity.AddComponent<DebugBox>(glm::vec4{ 0.3, 1.0, 0.1, 1 });
#endif

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

		// CHECK SIMULATION STATE
		if(simulate)
		{
			fixedDTCounter += dt;
			if (fixedDTCounter >= 0.005)
			{
				OnFixedUpdate(fixedDTCounter);
				fixedDTCounter = 0;
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
#ifdef _DEBUG
		{
			auto view = m_Registry.view<Transform, DebugBox>();
			for (auto& entity : view)
			{
				auto& [transform, DebugComp] = view.get<Transform, DebugBox>(entity);
				Renderer::SubmitLines(DebugComp.mat, DebugComp.va, transform, static_cast<int>(entity));
				
			}
		}
#endif

		
	}

	void Scene::OnFixedUpdate(const double& dt)
	{
		physicsWorld->Tick(dt);
		auto view = m_Registry.view<Transform, Rigidbody>();
		for (auto& entt : view)
		{
			btTransform btransform;
			
			auto& [transform, rigidbody] = view.get<Transform, Rigidbody>(entt);
			rigidbody.internal_rb->getMotionState()->getWorldTransform(btransform);
			//rigidbody.internal_rb->transform
			btVector3 vec = btransform.getOrigin();
			transform.position = { vec.x(), vec.y(), vec.z() };
			btScalar rot[3];
			btransform.getRotation().getEulerZYX(rot[1], rot[0], rot[2]);
			transform.euler_rotation.x = glm::degrees(rot[0]);
			transform.euler_rotation.y = glm::degrees(rot[1]);
			transform.euler_rotation.z = glm::degrees(rot[2]);

		}
	}

	
	
}
