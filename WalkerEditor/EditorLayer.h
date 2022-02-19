#pragma once

#include "Walker.h"


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
	};
}