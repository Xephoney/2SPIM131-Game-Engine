#pragma once
#include <memory>

#include "Engine/Scene/Entity.h"

namespace Engine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

		void SetContext(const std::shared_ptr<Scene>& scene);
		void PickEntity(const uint32_t& EntityID);
		void OnImGuiRender();
	private:

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		std::shared_ptr<Scene> m_scene;

		Entity m_Selected;
	};
}
