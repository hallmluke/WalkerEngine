#include "walkerpch.h"
#include "RenderGraph.h"

#include "Renderer/RenderPasses/ShadowMapPass.h"
#include "Renderer/RenderPasses/GBufferPBRPass.h"
#include "Renderer/RenderPasses/DeferredPBRLightingPass.h"
#include "Renderer/RenderPasses/BoxBlurPass.h";
#include "Renderer/RenderPasses/DepthOfFieldPass.h"

namespace Walker {

	RenderGraph::RenderGraph(uint32_t viewportWidth, uint32_t viewportHeight)
		: m_ViewportWidth(viewportWidth), m_ViewportHeight(viewportHeight)
	{
		m_RenderPasses = std::vector<std::shared_ptr<RenderPass>>();
		std::shared_ptr<RenderPass> shadowMapPass = std::make_shared<ShadowMapPass>();
		std::shared_ptr<RenderPass> gBufferPBRPass = std::make_shared<GBufferPBRPass>(m_ViewportWidth, m_ViewportHeight);
		std::shared_ptr<RenderPass> deferredPBRLightingPass = std::make_shared<DeferredPBRLightingPass>(m_ViewportWidth, m_ViewportHeight);
		std::shared_ptr<RenderPass> boxBlurPass = std::make_shared<BoxBlurPass>(m_ViewportWidth, m_ViewportHeight);
		std::shared_ptr<RenderPass> depthOfFieldPass = std::make_shared<DepthOfFieldPass>(m_ViewportWidth, m_ViewportHeight);
		Link(gBufferPBRPass->GetOutput("gPosition"), deferredPBRLightingPass->GetInput("gPosition"));
		Link(gBufferPBRPass->GetOutput("gNormal"), deferredPBRLightingPass->GetInput("gNormal"));
		Link(gBufferPBRPass->GetOutput("gAlbedo"), deferredPBRLightingPass->GetInput("gAlbedo"));
		Link(gBufferPBRPass->GetOutput("gMetRoughAO"), deferredPBRLightingPass->GetInput("gMetRoughAO"));
		Link(deferredPBRLightingPass->GetOutput("gColor"), boxBlurPass->GetInput("u_ColorTexture"));
		Link(gBufferPBRPass->GetOutput("gPosition"), depthOfFieldPass->GetInput("u_Position"));
		Link(deferredPBRLightingPass->GetOutput("gColor"), depthOfFieldPass->GetInput("u_InFocusColor"));
		Link(boxBlurPass->GetOutput("gColor"), depthOfFieldPass->GetInput("u_OutOfFocusColor"));

		m_RenderPasses.push_back(shadowMapPass);
		m_RenderPasses.push_back(gBufferPBRPass);
		m_RenderPasses.push_back(deferredPBRLightingPass);
		m_RenderPasses.push_back(boxBlurPass);
		m_RenderPasses.push_back(depthOfFieldPass);
	}

	RenderGraph::~RenderGraph()
	{
	}
	/*void RenderGraph::AddRenderPass(RenderPass renderPass)
	{
	}*/
	/*std::vector<RenderPass> RenderGraph::GetRenderPasses()
	{
		return m_RenderPasses;
	}*/

	void RenderGraph::DrawScene(Scene& scene) const
	{
		// TODO: Determine draw order based on linked inputs/outputs
		for (auto renderPass : m_RenderPasses) {
			renderPass->DrawScene(scene);
		}
	}
	void RenderGraph::Link(RenderPassOutput output, RenderPassInput input)
	{
		input.RenderPass->LinkToInput(input.Name, output);
	}
}
