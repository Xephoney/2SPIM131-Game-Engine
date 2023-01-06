#include "engpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Engine/Particle/Emitter.h"
#include "Engine/Renderer/Renderer.h"

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

#define SIZE 16.f

namespace Engine
{
	Scene::Scene() 
	{
		physicsWorld = new PhysicsWorld();
		physicsWorld->Initialize();
		particleSystem = new ParticleSystem();
		particleSystem->init();


		Entity entity = CreateEmptyEntity("Directional Light");
		entity.AddComponent<DirectionalLight>();
		entity.GetComponent<Transform>().euler_rotation = { glm::radians(-83.f), glm::radians(16.f), glm::radians(27.f) };

		/*entity = CreateEntity("Cube 1");
		entity.GetComponent<Transform>().position = { -4, 20, 0 };
		entity.GetComponent<Transform>().euler_rotation = { 22.5, 45, 120 };
		auto rb = physicsWorld->CreateBoxBody(true,
			entity.GetComponent<Transform>().position,
			entity.GetComponent<Transform>().euler_rotation,
			entity.GetComponent<Transform>().scale / 2.f);
		entity.AddComponent<RigidBody>(rb);

		entity = CreateEntity("Cube 2");
		entity.GetComponent<Transform>().position = { 0, 20, 0 };
		entity.GetComponent<Transform>().euler_rotation = { 120, 12, 10 };
		rb = physicsWorld->CreateBoxBody(true,
			entity.GetComponent<Transform>().position,
			entity.GetComponent<Transform>().euler_rotation,
			entity.GetComponent<Transform>().scale / 2.f);
		entity.AddComponent<RigidBody>(rb);*/

		entity = CreateEntity("Floor");
		entity.GetComponent<Transform>().position = { 0, -0.25, 0 };
		entity.GetComponent<Transform>().scale = { 80, 0.5, 80 };
		auto rb = physicsWorld->CreateBoxBody(false,
			entity.GetComponent<Transform>().position,
			entity.GetComponent<Transform>().euler_rotation,
			entity.GetComponent<Transform>().scale / 2.f);
		entity.AddComponent<RigidBody>(rb);
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
		entity.AddComponent<DebugBox>(glm::vec4{ 0.3, 0.9, 0.1, 1 });
#endif

		return entity;
	}
	Entity Scene::CreateEmptyEntity(const std::string& tagName)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<Tag>(tagName);
		entity.AddComponent<Transform>(glm::mat4{ 1.f });
		entity.AddComponent<AudioSource>("Pop", entity.GetComponent<Transform>().position);
		entity.GetComponent<AudioSource>().playSound();
		return entity;
	}

	Entity Scene::CreateEmptyEmitterEntity(const std::string& tagName)
	{
		Entity entity = CreateEmptyEntity("Emitter");
		entity.AddComponent<EmitterComponent>();
		return entity;
	}

	bool Scene::DeleteEntity(Entity& entity)
	{
		if(entity.HasComponent<RigidBody>())
		{
			auto rb = entity.GetComponent<RigidBody>();
			physicsWorld->GetInterface().RemoveBody(rb.data);
			physicsWorld->GetInterface().DestroyBody(rb.data);
		}
		if (entity.HasComponent<AudioSource>())
		{
			auto ac = entity.GetComponent<AudioSource>();
			
			ac.swapSound("Delete");
			ac.playSound();
			
		}
		
		m_Registry.destroy(entity.m_EntityHandle);
		entity.m_EntityHandle= { entt::null };
		return true;
	}

	bool Scene::DeleteEntity(uint32_t entity)
	{
		Entity _entity = { static_cast<entt::entity>(entity), this };
		
		return DeleteEntity(_entity);
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

	void Scene::StartSimulation()
	{
		simulate = true;
		auto& view = m_Registry.view<Transform, RigidBody>();
		auto& interface = physicsWorld->GetInterface();
		for (auto& entity : view)
		{
			auto& [transform, rb] = view.get<Transform, RigidBody>(entity);
			interface.SetPosition(rb.data, { transform.position.x, transform.position.y, transform.position.z }, JPH::EActivation::Activate);
			interface.SetRotation(rb.data, JPH::Quat::sEulerAngles({ transform.euler_rotation.x, transform.euler_rotation.y, transform.euler_rotation.z}), JPH::EActivation::Activate);
			JPH::Shape* shape;
			if (rb.box)
				shape = new JPH::BoxShape({ transform.scale.x / 2.f, transform.scale.y / 2.f, transform.scale.z / 2.f });
			else
				shape = new JPH::SphereShape(transform.scale.x/2.f);

			interface.SetShape(rb.data, shape, true, JPH::EActivation::Activate);
		}
		physicsWorld->physics_system->OptimizeBroadPhase();

		auto& emitterView = m_Registry.view<Transform, EmitterComponent>();
		for (auto& entity : emitterView)
		{
			auto& [transform, emitter] = emitterView.get<Transform, EmitterComponent>(entity);
			Emitter n_emitter;
			n_emitter.CreateEmitter(transform.position, emitter.particle_properties.LifeTime);
			Particle particle;
			particle.init();
			n_emitter.PopulateEmitter(particle, 100, emitter.particle_properties);
			particleSystem->Emit(n_emitter);
			
		}

	}

	void Scene::StopSimulation()
	{
		simulate = false;
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

		if(simulate)
		{
			constexpr float timestep = 1.0 / 120.0;
			fixedDTCounter += dt;
			if(fixedDTCounter > timestep)
			{
				OnFixedUpdate(timestep * fixedDTCounter/timestep);
				fixedDTCounter = 0;
			}
			particleSystem->Render();
			particleSystem->Update(dt);
		}

		{
			auto view = m_Registry.view<Transform, DirectionalLight>();
			for (auto& e : view)
			{
				auto& [transform, light] = view.get<Transform, DirectionalLight>(view.front());

				transform.CalculateTransform();
				Renderer::SubmitDirectionalLight(
					*light.lightShader.get(),
					transform.Forward(),
					light.specularExponent,
					light.specularStrength,
					light.ambientStrength,
					light.lightColor);
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
					Renderer::Submit(mesh.material, mesh.vertexArray, transform, static_cast<int>(entity), staticmesh.color);
#ifdef _DEBUG
					Entity ent{ entity, this };
					if(ent.HasComponent<DebugBox>())
					{
						auto& DebugComp = ent.GetComponent<DebugBox>();
						Renderer::SubmitLines(DebugComp.mat, DebugComp.va, transform, static_cast<int>(entity));
					}
#endif
				}
			}
			
		}
	}

	void Scene::OnFixedUpdate(const double& dt)
	{
		physicsWorld->Tick(dt);
		auto view = m_Registry.view<Transform, RigidBody>();
		
		for (auto& entity : view)
		{
			auto& [transform, rb] = view.get<Transform, RigidBody>(entity);

			physicsWorld->GetBodyPosition(rb.data, transform.position);
			physicsWorld->GetBodyRotation(rb.data, transform.euler_rotation);
		}
	}

	void Scene::ShadowPass()
	{
		//Directional light SHADOWS AND UNIFORMS 
		{
			auto view = m_Registry.view<Transform, DirectionalLight>();
			
			for (auto& e : view)
			{
				auto& [transform, light] = view.get<Transform, DirectionalLight>(view.front());
				light.shadowFrameBuffer->Bind();
				transform.CalculateTransform();
				light.UpdateViewProjectionMatrix(transform.Forward());
				
				auto renderpass = m_Registry.view<Transform, StaticMeshRenderer>();
				for (auto& entity : renderpass)
				{
					auto& [renderTransform, staticmesh] = renderpass.get<Transform, StaticMeshRenderer>(entity);
					for (int i = 0; i < staticmesh.mesh.meshes.size(); i++)
					{
						auto mesh = MeshManager::instance().GetMeshFromID(staticmesh.mesh.meshes[i]);
						Renderer::SubmitDepthpass(light.shadowMapShader, mesh.vertexArray, renderTransform);
					}
				}
				light.shadowFrameBuffer->BindDepthTexture();
				//light.lightShader->Bind();
				light.lightShader->SetInt("shadowMap", light.shadowFrameBuffer->TextureID());
				light.shadowFrameBuffer->Unbind();
			}
		}

	}	
}
