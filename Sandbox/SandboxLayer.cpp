#include "SandboxLayer.h"

namespace Walker {

	SandboxLayer::SandboxLayer() : Layer("SandboxLayer")
	{

	}
	void SandboxLayer::OnAttach()
	{
		TextureSpecification spec;
		spec.Width = 512;
		spec.Height = 512;
		spec.WrapS = TextureWrapType::CLAMP_EDGE;
		spec.WrapT = TextureWrapType::CLAMP_EDGE;
		spec.MagFilter = TextureFilterType::LINEAR;
		spec.MinFilter = TextureFilterType::LINEAR;
		spec.TextureFormat = TextureFormat::RGBA32F;
		spec.Type = TextureType::FLOAT;

		m_ComputeTexture = Texture2D::Create(spec);

		m_ComputeShader = ComputeShader::Create("Comp", "Shaders/test.comp");

		m_DebugShader = Shader::Create("Debug", "Shaders/debug_quad.vert", "Shaders/debug_quad_input.frag");
	}
	void SandboxLayer::OnDetach()
	{
	}

	void SandboxLayer::OnUpdate(float ts)
	{
		m_ComputeShader->Bind();
		m_ComputeTexture->BindImage();
		m_ComputeShader->SetVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
		m_ComputeShader->Dispatch(m_ComputeTexture->GetWidth(), m_ComputeTexture->GetHeight(), 1);

		m_ComputeShader->Barrier();

		RenderCommand::Clear();
		m_DebugShader->Bind();
		m_ComputeTexture->Bind();
		m_DebugShader->SetInt("u_Input", 0);

		m_Quad.Draw();
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