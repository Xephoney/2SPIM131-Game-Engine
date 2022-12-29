#include "SceneHierarchyPanel.h"
#include <Engine/Scene/Components.h>
#include <imgui.h>

#include "Engine/Scene/Entity.h"
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "Engine/Physics/PhysicsWorld.h"
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
		/*ImGui::ShowDemoWindow();*/
		ImGui::Begin("Scene Graph");
		m_scene->m_Registry.each([&](entt::entity entity)
		{
			Entity e{ entity, m_scene.get() };
			DrawEntityNode(e);
		});

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
			m_Selected = {};
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
		bool opened = ImGui::TreeNodeEx((void*)(static_cast<uint64_t>(static_cast<uint32_t>(entity))), flags, tag.tag.c_str());
		if(ImGui::IsItemClicked())
		{
			m_Selected = entity;
		}
		if(opened)
		{
			ImGui::TreePop();
			//DrawEntityNode()
		}
	}
	bool positionDirtyFlag = true;
	bool rotationDirtyFlag = true;
	
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		
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
				positionDirtyFlag = ImGui::DragFloat3(" ", value_ptr(transform.position), 0.1f);
				ImGui::Text("Rotation"); ImGui::SameLine();
				rotationDirtyFlag = ImGui::DragFloat3("\t", value_ptr(transform.euler_rotation), 0.05f); ImGui::SameLine(); ImGui::Checkbox("                ", &rotationDirtyFlag);
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

		if (entity.HasComponent<Rigidbody>())
		{
			if (ImGui::TreeNodeEx((void*)(typeid(Rigidbody).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Rigidbody"))
			{
				auto& rb = entity.GetComponent<Rigidbody>();
				
				if (positionDirtyFlag)
					rb.internal_rb->getWorldTransform().setOrigin({ entity.GetComponent<Transform>().position.x,entity.GetComponent<Transform>().position.y,entity.GetComponent<Transform>().position.z });
				if(rotationDirtyFlag)
					rb.internal_rb->getWorldTransform().setRotation(btQuaternion{ glm::radians(entity.GetComponent<Transform>().euler_rotation.z),glm::radians(entity.GetComponent<Transform>().euler_rotation.y), glm::radians(entity.GetComponent<Transform>().euler_rotation.x)});
				if(ImGui::Checkbox("Dynamic", &rb.dynamic))
				{
					if(rb.mass > 0.0 && rb.dynamic)
					{
						btVector3 localinert = rb.internal_rb->getLocalInertia();
						rb.internal_rb->getCollisionShape()->calculateLocalInertia(rb.mass, localinert);
						rb.internal_rb->setMassProps(rb.mass, localinert);
						//rb.internal_rb->setAngularVelocity(btVector3{ 0,10,0 });
					}
					else
					{
						rb.internal_rb->setMassProps(0, btVector3(0,0,0));
					}
				}

				ImGui::Text("Mass  "); ImGui::SameLine();
				
				if (ImGui::DragFloat(" ##kjdsafjlkdsafdsa", &rb.mass, 1.f, 0.f, 200000.f))
					rb.internal_rb->setMassProps(rb.mass, { 0,0,0 });

				if(ImGui::Button("Reset Motion"))
				{
					rb.internal_rb->clearForces();
					rb.internal_rb->setLinearVelocity({ 0,0,0 });
					rb.internal_rb->setAngularVelocity({ 0,0,0 });
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		else if (ImGui::Button("Add RigidBody"))
		{
			auto& tf =  entity.GetComponent<Transform>();
			auto rb= m_scene->physicsWorld->NewRigidBody(1.f, tf.position, tf.euler_rotation, CollisionShape::Box);
			entity.AddComponent<Rigidbody>(rb);
		}
		ImGui::Separator();
	}
}
