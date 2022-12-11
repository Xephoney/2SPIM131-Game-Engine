#include "SceneHierarchyPanel.h"
#include <Engine/Scene/Components.h>
#include <imgui.h>


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

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Graph");
		m_scene->m_Registry.each([&](auto entity)
		{
			Entity e{ entity, m_scene->m_Registry, m_scene };
			DrawEntityNode(e);
		});
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		Tag& tag = entity.GetComponent<Tag>();

		ImGuiTreeNodeFlags flags = ((m_Selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) |  ImGuiTreeNodeFlags_OpenOnArrow;
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
}
