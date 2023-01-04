#include "SceneHierarchyPanel.h"
#include <Engine/Scene/Components.h>
#include <imgui.h>

#include "Engine/Scene/Entity.h"
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
//#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Random.h"

namespace Engine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const std::shared_ptr<Scene>& scene)
	{
		m_scene = scene;
	}

	void SceneHierarchyPanel::PickEntity(const uint32_t& EntityID)
	{
		m_Selected = { static_cast<entt::entity>(EntityID), m_scene.get() };
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("Scene Graph");

		if (ImGui::IsMouseDown(ImGuiMouseButton_Right) &&  ImGui::IsWindowHovered())
			ImGui::OpenPopup("create context");

		if (ImGui::BeginPopup("create context"))
		{
			ImGui::Text(" - Create Menu -" );
			ImGui::Separator();
			if (ImGui::MenuItem("Empty"))
			{
				if(m_Selected)
				{
					Entity ent = m_scene->CreateEmptyEntity("Empty Child");
					m_Selected.children.push_back(ent);
					m_Selected = ent;
				}
				else
				{
					m_Selected = m_scene->CreateEmptyEntity("Empty Child");
				}
			}
						
			ImGui::Separator();
			if (ImGui::BeginMenu("3D"))
			{
				if(ImGui::MenuItem("Cube"))
					m_Selected = m_scene->CreateEntity("Cube");
				ImGui::MenuItem("Sphere");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Audio"))
			{
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Physics"))
			{
				if (ImGui::MenuItem("Physics Box"))
				{
					m_Selected = m_scene->CreateEntity("Physics Box (dynamic)");
					glm::vec3 position, rotation, scale;
					auto& tf = m_Selected.GetComponent<Transform>();
					position = tf.position;
					rotation = tf.euler_rotation;
					scale = tf.scale;
					auto body = m_scene->physicsWorld->CreateBoxBody(true, position, rotation, scale / 2.f);
					m_Selected.AddComponent<RigidBody>(body);
				}
				if (ImGui::MenuItem("Physics Ball"))
				{
					Entity wreckingBall = m_scene->CreateEmptyEntity("Physics Ball");
					wreckingBall.AddComponent<StaticMeshRenderer>("../Engine/Assets/3D/Primitives/sphere.gltf");
					auto& tf = wreckingBall.GetComponent<Transform>();
					tf.position = { 0, 0, 0 };
					tf.scale = glm::vec3(1);
					wreckingBall.AddComponent<RigidBody>(m_scene->physicsWorld->CreateSphereBody(true, tf.position, { 0,0,0 }, 0.5f)).box = false;
					wreckingBall.GetComponent<StaticMeshRenderer>().color = { 0.8, 0.2, 0.1, 1 };
				}
				if (ImGui::MenuItem("Static Box"))
				{
					m_Selected = m_scene->CreateEntity("Physics Box (static)");
					glm::vec3 position, rotation, scale;
					auto& tf = m_Selected.GetComponent<Transform>();
					position = tf.position;
					rotation = tf.euler_rotation;
					scale = tf.scale;
					auto body = m_scene->physicsWorld->CreateBoxBody(false, position, rotation, scale / 2.f);
					m_Selected.AddComponent<RigidBody>(body).dynamic = false;
				}
				if(ImGui::MenuItem("Wrecking Ball"))
				{
					ConstructStructure();
					
					Entity wreckingBall = m_scene->CreateEmptyEntity("WRECKING BALL");
					wreckingBall.AddComponent<StaticMeshRenderer>("../Engine/Assets/3D/Primitives/sphere.gltf");
					auto& tf = wreckingBall.GetComponent<Transform>();
					tf.position = { 0, 30, 0 };
					tf.scale = glm::vec3(8);
					wreckingBall.AddComponent<RigidBody>(m_scene->physicsWorld->CreateSphereBody(true, tf.position, { 0,0,0 }, 8.f/2.f)).box = false;
					wreckingBall.GetComponent<StaticMeshRenderer>().color = { 0.8, 0.2, 0.1, 1 };
				}

				if(ImGui::MenuItem("Structure"))
				{
					ConstructStructure();
				}

				ImGui::EndMenu();
			}
				
			if (ImGui::BeginMenu("Particles"))
			{
				if (ImGui::MenuItem("Emitter"))
				{
					m_Selected = m_scene->CreateEmptyEmitterEntity("Emitter");
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Lighting"))
			{
				ImGui::MenuItem("Cube Map");
				ImGui::MenuItem("Directional Light");
				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		m_scene->m_Registry.each([&](entt::entity entity)
		{
			Entity e{ entity, m_scene.get() };
			DrawEntityNode(e);
		});

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
			m_Selected = {entt::null, m_scene.get()};

		// DELETE BUTTON 
		if(ImGui::IsKeyPressed(ImGuiKey_Delete, false) && m_Selected)
		{
			m_scene->DeleteEntity(m_Selected);
			m_Selected = { entt::null, m_scene.get() };
		}
		

		ImGui::End();


		ImGui::Begin("Details");
		if(m_Selected.m_EntityHandle != entt::null)
		{
			DrawComponents(m_Selected);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		Tag& tag = entity.GetComponent<Tag>();

		ImGuiTreeNodeFlags flags = ((m_Selected && m_Selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) |  ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(static_cast<uint64_t>(static_cast<uint32_t>(entity))), flags, tag.tag.c_str());
		if(ImGui::IsItemClicked())
		{
			m_Selected = entity;
		}
		if(opened)
		{
			for(auto& entity : entity.children)
			{
				DrawEntityNode(entity);
			}
			ImGui::TreePop();
			
		}
	}

	void SceneHierarchyPanel::ConstructStructure()
	{
		Entity temp_entity;
		int xz_size = 5;
		int height = 10;
		for (int y = 0; y <= height; y++)
		{
			int amount = (y % 2 == 0) ? xz_size : xz_size - 1;
			float colorval = ((float)y / (float)height) + 0.25f;
			for (int x = 0; x < amount; x++)
			{
				temp_entity = m_scene->CreateEntity("Physics Box (dynamic)");
				glm::vec3 position, rotation, scale;
				auto& tf = temp_entity.GetComponent<Transform>();

				tf.position.z = static_cast<float>(xz_size) / 2.f + 0.1f;
				tf.position.x = static_cast<float>(x) - (static_cast<float>(amount) / 2.f) + 0.1f;
				tf.position.x += 0.5f;
				tf.position.y = 0.51f + static_cast<float>(y);

				position = tf.position;
				rotation = tf.euler_rotation;
				scale = tf.scale;
				auto body = m_scene->physicsWorld->CreateBoxBody(true, position, rotation, scale / 2.f);
				temp_entity.AddComponent<RigidBody>(body);
				temp_entity.GetComponent<StaticMeshRenderer>().color = { colorval,colorval,colorval,1 };

				temp_entity = m_scene->CreateEntity("Physics Box (dynamic)");
				auto& tf2 = temp_entity.GetComponent<Transform>();

				tf2.position.z = static_cast<float>(-xz_size) / 2.f + 0.1f;
				tf2.position.x = static_cast<float>(x) - (static_cast<float>(amount) / 2.f) + 0.1f;
				tf2.position.x += 0.5f;
				tf2.position.y = 0.51f + static_cast<float>(y);

				position = tf2.position;
				rotation = tf2.euler_rotation;
				scale = tf2.scale;
				body = m_scene->physicsWorld->CreateBoxBody(true, position, rotation, scale / 2.f);
				temp_entity.AddComponent<RigidBody>(body);
				temp_entity.GetComponent<StaticMeshRenderer>().color = { colorval,colorval,colorval,1 };
			}
			amount = (y % 2 == 1) ? xz_size : xz_size - 1;
			for (int z = 0; z < amount; z++)
			{
				temp_entity = m_scene->CreateEntity("Physics Box (dynamic)");
				glm::vec3 position, rotation, scale;
				auto& tf = temp_entity.GetComponent<Transform>();

				tf.position.x = static_cast<float>(xz_size) / 2.f + 0.1f;
				tf.position.z = static_cast<float>(z) - (static_cast<float>(amount) / 2.f) + 0.1f;

				tf.position.z += 0.5f;
				tf.position.y = 0.51f + static_cast<float>(y);

				position = tf.position;
				rotation = tf.euler_rotation;
				scale = tf.scale;
				auto body = m_scene->physicsWorld->CreateBoxBody(true, position, rotation, scale / 2.f);
				temp_entity.AddComponent<RigidBody>(body);
				temp_entity.GetComponent<StaticMeshRenderer>().color = { colorval,colorval,colorval,1 };

				temp_entity = m_scene->CreateEntity("Physics Box (dynamic)");
				auto& tf2 = temp_entity.GetComponent<Transform>();
				

				tf2.position.x = static_cast<float>(-xz_size) / 2.f + 0.1;
				tf2.position.z = static_cast<float>(z) - (static_cast<float>(amount) / 2.f) + 0.1f;

				tf2.position.z += 0.5f;
				tf2.position.y = 0.51f + static_cast<float>(y);

				position = tf2.position;
				rotation = tf2.euler_rotation;
				scale = tf2.scale;
				body = m_scene->physicsWorld->CreateBoxBody(true, position, rotation, scale / 2.f);
				temp_entity.AddComponent<RigidBody>(body);
				temp_entity.GetComponent<StaticMeshRenderer>().color = { colorval,colorval,colorval,1 };
			}
		}
	}

	bool positionDirtyFlag = true;
	bool rotationDirtyFlag = true;
	
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.m_EntityHandle == entt::null)
			return;
		if (entity.HasComponent<Tag>())
		{
			auto& nameTag = entity.GetComponent<Tag>().tag;

			char buffer[512] = {};
			strcpy_s(buffer, sizeof(buffer), nameTag.c_str());

			ImGui::Text("Name "); ImGui::SameLine();
			if(ImGui::InputText(" ",buffer, sizeof(buffer)))
			{
				nameTag = std::string(buffer);
			}


			ImGui::Separator();
		}

		if(entity.HasComponent<Transform>())
		{
			auto& transform = entity.GetComponent<Transform>();
			if (ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(Transform).hash_code()),ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				ImGui::Text("Position"); ImGui::SameLine();
				ImGui::DragFloat3(" ", value_ptr(transform.position), 0.1f);
				ImGui::Text("Rotation"); ImGui::SameLine();

				glm::vec3 rot = { glm::degrees(transform.euler_rotation.x), glm::degrees(transform.euler_rotation.y), glm::degrees(transform.euler_rotation.z )};
				if (ImGui::DragFloat3("\t", glm::value_ptr(rot), 1.f))
				{
					transform.euler_rotation.x = glm::radians(rot.x);
					transform.euler_rotation.y = glm::radians(rot.y);
					transform.euler_rotation.z = glm::radians(rot.z);
				}
				ImGui::Text("Scale   "); ImGui::SameLine();
				static bool uniformScale = false;
				if(!uniformScale)
				{
					ImGui::DragFloat3(" \t", value_ptr(transform.scale), 0.05f);
				}
				else
				{
					static float uniScale = 1;
					if (ImGui::DragFloat("  \t", &uniScale, 0.05f))
					{
						transform.scale.x = uniScale;
						transform.scale.y = uniScale;
						transform.scale.z = uniScale;
					}
				}
				ImGui::Checkbox("Uniform", &uniformScale);
				ImGui::TreePop();
			}
						
			ImGui::Separator();
		}

		if(entity.HasComponent<AudioSource>())
		{
			auto& audioCmp = entity.GetComponent<AudioSource>();
			if(ImGui::TreeNodeEx((void*)(typeid(AudioSource).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Audio Source"))
			{
				if (ImGui::Button("Play Sound"))
					audioCmp.playSound();

				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		if(entity.HasComponent<StaticMeshRenderer>())
		{
			auto& smr = entity.GetComponent<StaticMeshRenderer>();
			
			if (ImGui::TreeNodeEx((void*)(typeid(StaticMeshRenderer).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Static Mesh"))
			{
				auto meshes = smr.GetMeshes();
				ImGui::Text("Meshes");
				ImGui::Separator();
				for (auto& mesh : meshes)
				{
					ImGui::BulletText("Shader"); ImGui::SameLine();
					ImGui::TextColored({ 0.5f,0.7f,0.2f,1.f }, mesh.material.shader->GetName().c_str());
					ImGui::BulletText("Diffuse"); ImGui::SameLine();
					ImGui::ColorEdit3("##label", glm::value_ptr(smr.color));
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		if (entity.HasComponent<RigidBody>())
		{
			if (ImGui::TreeNodeEx((void*)(typeid(RigidBody).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Rigidbody"))
			{
				auto& rb = entity.GetComponent<RigidBody>();

				auto& interface = m_scene->physicsWorld->GetInterface();
				
				if(ImGui::Checkbox("Dynamic", &rb.dynamic))
				{
					if(rb.dynamic)
					{
						interface.SetMotionType(rb.data, JPH::EMotionType::Dynamic, JPH::EActivation::Activate);
					}
					else
					{
						interface.SetMotionType(rb.data, JPH::EMotionType::Static, JPH::EActivation::Activate);
					}
				}

				//Collider Extent
				if (ImGui::Button("Reset Motion"))
				{
					interface.SetLinearAndAngularVelocity(
						rb.data,
						{ 0,0,0 },
						{ 0,0,0 });
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		else if (ImGui::Button("Add RigidBody"))
		{
			auto& tf = entity.GetComponent<Transform>();
			auto rb = m_scene->physicsWorld->CreateBoxBody(true, tf.position, tf.euler_rotation, { 0.5f,0.5f ,0.5f });
			entity.AddComponent<RigidBody>(rb);
		}
		ImGui::Separator();

		if (entity.HasComponent<DirectionalLight>())
		{
			auto& dl = entity.GetComponent<DirectionalLight>();
			ImGui::Text("Light Color"); ImGui::SameLine();
			ImGui::ColorEdit3("##label0", glm::value_ptr(dl.lightColor));
			ImGui::Text("Ambient Intensity"); ImGui::SameLine();
			ImGui::DragFloat("##label1", &dl.ambientStrength, 0.01f, 0.0f, 1.0f);

			ImGui::Text("Specular Strength"); ImGui::SameLine();
			ImGui::DragFloat("##label2", &dl.specularStrength, 0.05f, 0.0f, 1.0f);
			ImGui::Text("Specular Exponent"); ImGui::SameLine();
			ImGui::DragInt("##label3", &dl.specularExponent, 4, 0, 512);
			
		}

		if(entity.HasComponent<EmitterComponent>())
		{
			auto& emitter = entity.GetComponent<EmitterComponent>();
			if (ImGui::TreeNodeEx((void*)(typeid(EmitterComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "EmitterComponent"))
			{
				ImGui::Text("Lifetime"); ImGui::SameLine();
				ImGui::DragFloat("##label098586767835436176451", &emitter.particle_properties.LifeTime,0.2f, 0.f, 10.f);

				ImGui::Text("Start Color"); ImGui::SameLine();
				ImGui::ColorEdit4("##label9038217", glm::value_ptr(emitter.particle_properties.ColorBegin));
				ImGui::Text("End Color"); ImGui::SameLine();
				ImGui::ColorEdit4("##label9098493503", glm::value_ptr(emitter.particle_properties.ColorEnd));
				ImGui::Separator();
				ImGui::Text("Start Velocity"); ImGui::SameLine();
				ImGui::DragFloat3("##label5894728906432654782354802", glm::value_ptr(emitter.particle_properties.Velocity));
				ImGui::Text("Velocity Variation"); ImGui::SameLine();
				ImGui::DragFloat3("##label58947289064326547823542", glm::value_ptr(emitter.particle_properties.VelocityVariation),0.2f);
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
	}
}
