#include "SandboxLayer.h"
#include "Renderer/RenderPasses/DeferredPBRLightingPass.h"

#include <imgui.h>

namespace Walker {

	SandboxLayer::SandboxLayer() : Layer("SandboxLayer")
	{

	}
	void SandboxLayer::OnAttach()
	{
		m_ActiveScene = std::make_shared<Scene>();
		std::shared_ptr<Model> sponzaPBR = std::make_shared<Model>("SponzaPBR", "Models/SponzaPBR/Sponza.gltf", m_ActiveScene.get());
	}
	void SandboxLayer::OnDetach()
	{
	}

	void SandboxLayer::OnUpdate(float ts)
	{
		m_ActiveScene->OnUpdate(ts);

	}
	void SandboxLayer::OnImGuiRender()
	{
	}
	void SandboxLayer::OnEvent(Event& e)
	{
	}
	bool SandboxLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		return false;
	}
	bool SandboxLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}
	void SandboxLayer::NewScene()
	{
	}
	void SandboxLayer::OpenScene()
	{
	}
	void SandboxLayer::OpenScene(const std::filesystem::path& path)
	{
	}
	void SandboxLayer::SaveSceneAs()
	{
	}
}