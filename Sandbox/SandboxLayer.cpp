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

		m_PointLight = m_ActiveScene->CreateEntity("Default Pointlight");
		auto& pointLightComponent = m_PointLight.AddComponent<PointLightComponent>();
		auto& pointLightPosition = m_PointLight.GetComponent<TransformComponent>();


		pointLightPosition.Translation = glm::vec3(0.0, 1.0, 0.0);
		pointLightComponent.PointLightPtr->m_DiffuseIntensity = 7.0f;
		std::shared_ptr<Model> sponzaPBR = std::make_shared<Model>("SponzaPBR", "Models/SponzaPBR/Sponza.gltf", m_ActiveScene.get());

		auto giProbe = m_ActiveScene->CreateEntity("GI Probe");
		auto& giProbeComponent = giProbe.AddComponent<GIProbeComponent>();
		auto& giProbeTransform = giProbe.GetComponent<TransformComponent>();

		giProbeComponent.Probe->Subdiv = 256;
		giProbeTransform.Translation = glm::vec3(0.0f, 10.0f, 0.0f);
		giProbeTransform.Scale = glm::vec3(90.0f, 90.0f, 90.0f);
		/*RenderGraphSpecification spec(
			{ // Passes
				{RenderPassType::ShadowMapPass, "ShadowMapPass"},
				{RenderPassType::GBufferPBRPass, "GBufferPBRPass"},
				{RenderPassType::DeferredPBRLightingPass, "DeferredPBRLightingPass"},
				{RenderPassType::BloomCompute, "BloomCompute"},
				{RenderPassType::TonemapCompute, "TonemapCompute"}
				//{RenderPassType::BoxBlurPass, "BoxBlurPass"},
				//{RenderPassType::DepthOfFieldPass, "DepthOfFieldPass"} 
			},
			{ // Links
				{ "GBufferPBRPass", "gPosition", "DeferredPBRLightingPass", "gPosition" },
				{ "GBufferPBRPass", "gNormal", "DeferredPBRLightingPass", "gNormal" },
				{ "GBufferPBRPass", "gAlbedo", "DeferredPBRLightingPass", "gAlbedo" },
				{ "GBufferPBRPass", "gMetRoughAO", "DeferredPBRLightingPass", "gMetRoughAO" },
				{ "DeferredPBRLightingPass", "gColor", "BloomCompute", "img_input" },
				{ "DeferredPBRLightingPass", "gColor", "TonemapCompute", "img_input"},
				{ "BloomCompute", "img_output", "TonemapCompute", "bloom_input"}
				//{ "DeferredPBRLightingPass", "gColor", "BoxBlurPass", "u_ColorTexture" },
				//{ "GBufferPBRPass", "gPosition", "DepthOfFieldPass", "u_Position" },
				//{ "DeferredPBRLightingPass", "gColor", "DepthOfFieldPass", "u_InFocusColor" },
				//{ "BoxBlurPass", "gColor", "DepthOfFieldPass", "u_OutOfFocusColor" } 
			});*/
		RenderGraphSpecification spec(
			{
				{ RenderPassType::ShadowMapPass, "ShadowMapPass"},
				{ RenderPassType::Voxelization, "VoxelizationPass "},
				{ RenderPassType::VoxelVisualization, "VoxelVisualization"}
			},
			{

			});

		/*RenderGraphSpecification spec(
			{
				{ RenderPassType::ShadowMapPass, "ShadowMapPass"},
				{ RenderPassType::Voxelization, "VoxelizationPass"},
				{ RenderPassType::GBufferPBRPass, "GBufferPBRPass"},
				{ RenderPassType::DeferredPBRLightingPass, "DeferredPBRLightingPass"},
				{ RenderPassType::BloomCompute, "BloomCompute"},
				{ RenderPassType::TonemapCompute, "TonemapCompute"}
			},
			{
				{ "GBufferPBRPass", "gPosition", "DeferredPBRLightingPass", "gPosition" },
				{ "GBufferPBRPass", "gNormal", "DeferredPBRLightingPass", "gNormal" },
				{ "GBufferPBRPass", "gAlbedo", "DeferredPBRLightingPass", "gAlbedo" },
				{ "GBufferPBRPass", "gMetRoughAO", "DeferredPBRLightingPass", "gMetRoughAO" },
				{ "VoxelizationPass", "VoxelTex", "DeferredPBRLightingPass", "VoxelTex" },
				{ "DeferredPBRLightingPass", "gColor", "BloomCompute", "img_input" },
				{ "DeferredPBRLightingPass", "gColor", "TonemapCompute", "img_input"},
				{ "BloomCompute", "img_output", "TonemapCompute", "bloom_input"}
			}
			);*/

		m_RenderGraph = std::make_shared<RenderGraph>(spec, 1600, 900);
		/*TextureSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		spec.WrapS = TextureWrapType::CLAMP_EDGE;
		spec.WrapT = TextureWrapType::CLAMP_EDGE;
		spec.MagFilter = TextureFilterType::LINEAR;
		spec.MinFilter = TextureFilterType::LINEAR;
		spec.TextureFormat = TextureFormat::RGBA16F;
		spec.Type = TextureType::FLOAT;

		m_ComputeTexture = Texture2D::Create(spec);

		spec.Width = spec.Width / 2;
		spec.Height = spec.Height / 2;

		m_Half = Texture2D::Create(spec);

		m_ComputeShader = ComputeShader::Create("Comp", "Shaders/test.comp");
		m_Prefilter = ComputeShader::Create("BloomPrefilter", "Shaders/bloomprefilter.comp");*/

		m_DebugShader = Shader::Create("Debug", "Shaders/debug_quad.vert", "Shaders/debug_quad_input.frag");
	}
	void SandboxLayer::OnDetach()
	{
	}

	void SandboxLayer::OnUpdate(float ts)
	{
		/*m_ComputeShader->Bind();
		m_ComputeTexture->BindImage();
		m_ComputeShader->SetVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
		m_ComputeShader->Dispatch(m_ComputeTexture->GetWidth(), m_ComputeTexture->GetHeight(), 1);

		m_ComputeShader->Barrier();

		m_Half->BindImage(1);
		m_Prefilter->Bind();
		m_Prefilter->Dispatch(m_Half->GetWidth(), m_Half->GetHeight(), 1);
		m_Prefilter->Barrier();

		RenderCommand::Clear();
		m_DebugShader->Bind();
		m_Half->Bind();
		m_DebugShader->SetInt("u_Input", 0);

		m_Quad.Draw();*/

		m_ActiveScene->OnUpdate(ts);
		m_RenderGraph->DrawScene(*m_ActiveScene);
		/*RenderCommand::BindDefaultFramebuffer();

		RenderCommand::Clear();
		m_DebugShader->Bind();
		m_RenderGraph->GetRenderPass("TonemapCompute")->BindOutput(2, 0);
		
		//m_RenderGraph->GetRenderPass("DeferredPBRLightingPass")->BindOutput(0, 0);
		
		m_DebugShader->SetInt("u_Input", 0);
		m_Quad.Draw();*/

	}
	void SandboxLayer::OnImGuiRender()
	{
		auto passes = m_RenderGraph->GetRenderPasses();

		for (auto pass : passes) {
			pass->OnImGuiRender();
		}

		ImGui::Begin("Point light");
		if (m_PointLight && m_PointLight.HasComponent<PointLightComponent>()) {
			auto& pointLightComponent = m_PointLight.GetComponent<PointLightComponent>();
			auto& pointLight = pointLightComponent.PointLightPtr;

			ImGui::DragFloat("Ambient Intensity", &(pointLight->m_AmbientIntensity), 0.1f, 0.0f, 10.0f);
			ImGui::DragFloat("Diffuse Intensity", &(pointLight->m_DiffuseIntensity), 0.1f, 0.0f, 10.0f);
			ImGui::DragFloat("Specular Intensity", &(pointLight->m_SpecularIntensity), 0.1f, 0.0f, 10.0f);
		}

		ImGui::End();
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