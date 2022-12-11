#pragma once
#include <memory>

#include "Engine/Scene/Scene.h"

namespace Engine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

		void SetContext(const std::shared_ptr<Scene>& scene);

		void OnImGuiRender();
	private:

		void DrawEntityNode(Entity entity);
		std::shared_ptr<Scene> m_scene;

		Entity m_Selected;
	};
}
