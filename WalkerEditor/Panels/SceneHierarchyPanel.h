#pragma once
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace Walker {

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene> scene);

		void OnImGuiRender();

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		void SetContext(const std::shared_ptr<Scene>& scene);
		void SetSelectedEntity(Entity entity);
	private:
		std::shared_ptr<Scene> m_Context;
		Entity m_SelectionContext;
	};
}