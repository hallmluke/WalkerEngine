#pragma once

#include "Walker.h"

#include <filesystem>

namespace Walker {

	class SandboxLayer : public Layer
	{
	public:
		SandboxLayer();
		virtual ~SandboxLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(float ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();

	private:
		std::shared_ptr<ComputeShader> m_ComputeShader;
		std::shared_ptr<Texture> m_ComputeTexture;
		std::shared_ptr<Scene> m_ActiveScene;
		std::shared_ptr<RenderGraph> m_RenderGraph;
		Quad m_Quad;
		std::shared_ptr<Shader> m_DebugShader;
	};
}