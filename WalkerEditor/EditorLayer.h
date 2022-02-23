#pragma once

#include "Walker.h"
#include "Panels/SceneHierarchyPanel.h"


namespace Walker {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(float ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event & e) override;

	private:
		std::shared_ptr<Scene> m_ActiveScene;
		std::shared_ptr<RenderGraph> m_RenderGraph;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}