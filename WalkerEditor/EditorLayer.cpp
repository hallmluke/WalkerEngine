#include "EditorLayer.h"
#include "imgui.h"

namespace Walker {

	EditorLayer::EditorLayer() : Layer("EditorLayer")
	{
	}
	void EditorLayer::OnAttach()
	{
		m_ActiveScene = std::make_shared<Scene>();
		m_RenderGraph = std::make_shared<RenderGraph>(1280, 720);
	}
	void EditorLayer::OnDetach()
	{
	}
	void EditorLayer::OnUpdate(float ts)
	{
	}
	void EditorLayer::OnImGuiRender()
	{
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		uint32_t textureId = m_RenderGraph->GetOutputRendererId();
		ImGui::Image((void*)textureId, ImVec2{ 1280, 720 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
	}
	void EditorLayer::OnEvent(Event& e)
	{
	}
}