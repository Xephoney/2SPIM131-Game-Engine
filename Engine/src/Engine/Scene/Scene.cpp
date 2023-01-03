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
	Entity Scene::CreateEmptyEntity(const std::string& tagName)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<Tag>(tagName);
		entity.AddComponent<Transform>(glm::mat4{ 1.f });
		return entity;
	}

	Entity Scene::CreateEmptyEmitterEntity(const std::string& tagName)
	{
		Entity entity = CreateEmptyEntity("Emitter");
		entity.AddComponent<EmitterComponent>();
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
			JPH::Shape* box = new JPH::BoxShape({ transform.scale.x / 2.f, transform.scale.y / 2.f, transform.scale.z / 2.f });
			interface.SetShape(rb.data, box, true, JPH::EActivation::Activate);
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
			particle.setParticleProperties(emitter.particle_properties);
			n_emitter.PopulateEmitter(particle, 100);
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
		// CHECK SIMULATION STATE
		if(simulate)
		{
			constexpr float timestep = 1.0 / 120.0;
			fixedDTCounter += dt;
			while(fixedDTCounter >= timestep)
			{
				OnFixedUpdate(fixedDTCounter);
				fixedDTCounter -= timestep;
			}



			particleSystem->Render();
			particleSystem->Update(dt);
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
				
				
				
				/*Renderer::SubmitDirectionalLight(
					*light.lightShader.get(),
					transform.Forward(),
					light.specularExponent,
					light.specularStrength,
					light.ambientStrength,
					light.lightColor);*/

				/*Renderer::SubmitDirectionalLightShadow(
					*light.lightShader.get(),
					transform.Forward(),
					light.lightSpaceMatrix,
					light.specularExponent,
					light.specularStrength,
					light.ambientStrength,
					light.lightColor);
					*/

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
				light.lightShader->SetInt("shadowMap", light.shadowFrameBuffer->TextureID());
				light.shadowFrameBuffer->Unbind();
			}
		}

	}	
}
